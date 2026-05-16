#include "stdafx.h"
#include "PlayerAttackBaseState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"

#include "Src/Actor/Character/Status/AttackParameterTable.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"
#include "PresentDamageIndicator.h"
#include "Boss.h"

namespace
{
	const auto ATTACK_END_FRAME = 5;			//! 攻撃終了フレーム。
	const auto RUSH_COMBO_THRESHOLD = 2;		//! 連続攻撃の閾値。
	const auto HIT_STOP_FRAME = 8;              //! ヒットストップのフレーム数。
	const auto DAMAGE_TEXT_OFFSET_Y = 120.0f;   //! ダメージテキストのY軸オフセット。
	const auto CRITICAL_PERCENTAGE = 100.0f;    //! クリティカル発生確立。
}
namespace nsApp
{
	namespace nsState
	{
		void PlayerAttackBaseState::Enter()
		{
			/* 攻撃の種類ごとにキャストを行う。*/
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 共通の初期化。*/
			OnCommonInitializeToEnter();

			/* アニメーションの再生と固有の初期化は派生クラスに譲渡。*/
			PlayAttackAnimation(); 
			OnEnterAttack();
		}


		void PlayerAttackBaseState::Update()
		{
			/* タイマーを加算する。*/
			m_attackTimer++;

			/* 入力クラスを取得する。*/
			const auto& inputClass = m_player->GetInputClass();


			// @TODO: リファ。
/////////////////////////////////////////////////////////////////////////////////////////////////

			/* Bボタンアクション。*/
			if (inputClass.IsAttack())
			{
				/* 
				 *タイマーを加算する。
				 * Bボタンを押すごとにタイマーを加算し、当てはまるなら連続攻撃に繋げる。
				 */
				m_rushCount++;
				
				/* Bボタンが押されていたら予約を入れる。*/
				m_inputRequests[ComboInputType::PressB] = true;
			}

			if (inputClass.IsPressX())
				m_inputRequests[ComboInputType::PressX] = true;

			/* Aボタンアクション。*/
			if (inputClass.IsSlashUp())
				m_inputRequests[ComboInputType::PressLB2] = true;
			else if(inputClass.IsJump())
				m_inputRequests[ComboInputType::PressA] = true;


			if (m_rushCount >= 2)
				m_inputRequests[ComboInputType::RushB] = true;

			/* 毎フレームの処理とタイミングがある処理を派生クラスに譲渡。*/
			if (OnUpdateAttack())
				return;

			OnAttackTick();

			/* 終了判定。*/
			if (m_attackTimer > 5 && !m_player->IsPlayAnimation())
			{
				/* Idle状態へ遷移。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
				return;
			}

			/* 衝突判定。*/
			if (!m_isHit)
			{
				auto& hitDetection = m_player->GetWeaponHitDetection();
				auto boss = FindGO<nsActor::Boss>("boss");

				if (boss != nullptr && hitDetection.IsHit(boss)) // ★本当に当たった時だけ！
				{
					OnHitDamageText(boss);
					boss->ApplyDamage(m_finalDamage);
					m_player->SetHitStop(HIT_STOP_FRAME);
					boss->SetHitStop(HIT_STOP_FRAME);
				}

				/* 終了判定。*/
				if (m_attackTimer > ATTACK_END_FRAME && !m_player->IsPlayAnimation())
				{
					m_stateMachine->ChangeState(new PlayerIdleState());
					return;
				}
			}
///////////////////////////////////////////////////////////////////////////////////////////////////
		}


		void PlayerAttackBaseState::Exit()
		{
			/* Stateを抜ける際の処理。*/
			/* 効果音とかエフェクトとかの再生をストップさせる*/
			if (m_player)
			{
				/* 当たり判定を奪取する。*/
				m_player->GetWeaponHitDetection().Disable();

				/* SEの再生を止める。*/
				m_player->StopWeaponSE();
			}

			/* 子クラスの終了処理。*/
			OnExitAttack();
		}


		void PlayerAttackBaseState::OnHitDamageText(nsActor::ICharacter* target)
		{
			// m_player や target が無い時は何もしない
			if (!m_player || !target)
				return;

			/* --- ダメージ計算はそのまま --- */
			m_getPlayerPosition = m_player->GetPosition();
			m_forwardDirection = m_player->GetForwardVector();
			const auto& playerStatus = m_player->GetCharacterStatus().attack;
			const auto& attackParameter = AttackParameterTable::GetAttackParameter(m_currentAttackType);

			m_finalDamage = static_cast<int>(playerStatus.normalDamage * attackParameter.damageMultiplier);

			m_criticalRate = playerStatus.criticalRate + attackParameter.criticalRatel;
			if ((rand() % 100) < (m_criticalRate * 100.0f))
				m_finalDamage = static_cast<int>(m_finalDamage * playerStatus.criticalDamage);


			m_screenPosition = target->GetPosition();
			m_screenPosition.y += 120.0f;

			/* ダメージテキストを表示する。*/
			m_damageIndicator = NewGO<PresentDamageIndicator>(0, "DamageUI");
			m_damageIndicator->Init(m_finalDamage, m_screenPosition);
		}


		bool PlayerAttackBaseState::CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id)
		{
			/* playerクラスが存在するか検知。*/
			if (!m_player)
				return false;

			/* 地上にいるかどうかを確認。*/ 
			m_isGrounded = m_player->GetCharacterController().IsOnGround();

			/* ステートIDと地上にいるかどうかを検知させる。*/
			const auto& routes = ComboRouteTable::GetRoutes(currentStateID, m_isGrounded);

			for (const auto& route : routes)
			{
				/* テーブルから指定されたものを取り出す。*/
				m_isInputMatch = m_inputRequests[route.inputType];

				/* 時間と入力条件を満たしているか確認。*/
				if (m_attackTimer >= route.cancelTime && m_isInputMatch)
				{
					/* 条件を満たしている場合、次の状態へ遷移する。*/
					id = static_cast<uint8_t>(route.nextStateID);
					return true;
				}
			}
			return false;
		}
	}
}