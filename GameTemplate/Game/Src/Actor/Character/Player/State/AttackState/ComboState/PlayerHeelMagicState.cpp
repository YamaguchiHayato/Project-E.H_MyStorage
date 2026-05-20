#include "stdafx.h"
#include "PlayerHeelMagicState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Effect/EffectList.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerHeelMagicState::PlayAttackAnimation()
		{
			/* 攻撃の種類をセット。*/
			m_currentAttackType = AttackType::HeelMagic;

			/* 再生するアニメーションをセット。*/
			m_player->PlayWeaponAnimation(AttackType::HeelMagic);
		}


		void PlayerHeelMagicState::OnEnterAttack()
		{
			/* チャージレベルを取得。*/
			m_chargeLevel = static_cast<float>(m_player->GetEffectScale());

			/* Clamp。*/
			if (m_chargeLevel <= 0.0f)
				m_chargeLevel = 1.0f;
		}


		void PlayerHeelMagicState::OnAttackTick()
		{
			/* 25フレーム目で効果発動 */
			if (m_attackTimer == 25) 
			{
				/* 回復エフェクト(回復本体)。*/
				PlayHeelMagicEffect();
				/* 回復エフェクト(パーティクル)。*/
				PlayHeelMagicParticleEffect();
				/* エリアヒール。*/
				ExecuteAreaHeal();
			}
		}


		bool PlayerHeelMagicState::OnUpdateAttack()
		{
			/* エフェクトの拡大。*/ 
			ComputeHeelEffectScale();

			/* 攻撃終了の判定。*/ 
			if (m_attackTimer > 35 && !m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			return true;
		}


		void PlayerHeelMagicState::OnExitAttack()
		{
			/* 2種類のエフェクトの再生を止める。*/
			if (m_heelEffect != nullptr) {
				m_heelEffect->Stop();
				m_heelEffect = nullptr;
			}
			if (m_particleEffect != nullptr) {
				m_particleEffect->Stop();
				m_particleEffect = nullptr;
			}
		}


		void PlayerHeelMagicState::PlayHeelMagicEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_heelEffectPosition = m_player->GetPosition();
			m_heelEffectPosition.y += 2.0f;
			/* エフェクトを再生。*/
			m_heelEffect =  m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic, m_heelEffectPosition, Quaternion::Identity, Vector3::One  *20.0f);
		}


		void PlayerHeelMagicState::PlayHeelMagicParticleEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_particleEffectPosition = m_player->GetPosition();
			m_particleEffectPosition.y += 2.0f;
			/* エフェクトを再生。*/
			m_particleEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, m_particleEffectPosition, Quaternion::Identity, Vector3::One * 20.0f);		
		}


		void PlayerHeelMagicState::ExecuteAreaHeal()
		{
			if (!m_player)
				return;

			/* 回復量。*/
			m_healAmount = 500 * static_cast<int>(m_chargeLevel);

			/* 周囲の味方を探して回復*/
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };
			for (const char* name : playerNames)
			{
				auto otherPlayer = FindGO<nsActor::Player>(name);
				if (otherPlayer && otherPlayer != m_player && otherPlayer->GetCharacterStatus().hp.currentHP > 0)
				{
					m_distance = (otherPlayer->GetPosition() - m_player->GetPosition()).Length();
					if (m_distance <= 150.0f * m_chargeLevel) {
						otherPlayer->ApplyDamage(-m_healAmount);
					}
				}
			}

			/* 自分も回復。*/
			m_player->ApplyDamage(-m_healAmount);
		}
	}
}