#include "stdafx.h"
#include "PlayerMagicAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerMagicAttackState::PlayAttackAnimation()
		{
			/* 攻撃の種類をセット。*/
			m_currentAttackType = AttackType::MagicAttack;

			/* 再生するアニメーションの種類をセット。*/
			m_player->PlayWeaponAnimation(AttackType::MagicAttack);
		}


		void PlayerMagicAttackState::OnEnterAttack()
		{
			/* 当たり判定を付与。*/
			m_player->GetWeaponHitDetection().Enable();
		}


		void PlayerMagicAttackState::OnAttackTick()
		{
			/* 15フレーム目でレーザーエフェクトを生成 */
			if (m_attackTimer == 15)
				SpawnLaserEffect();
		}


		bool PlayerMagicAttackState::OnUpdateAttack()
		{
			/* 長いアニメーションなので、固有の終了条件を定義 */
			if (m_attackTimer > 40 && !m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			return true;
		}


		void PlayerMagicAttackState::OnExitAttack()
		{
			/* 終了時にエフェクトを安全に消去 */
			if (m_laserEffect != nullptr)
			{
				m_laserEffect->Stop();
				m_laserEffect = nullptr;
			}
		}


		void PlayerMagicAttackState::SpawnLaserEffect()
		{
			m_spawnPosition = m_player->GetPosition();
			m_spawnPosition.y += 10.0f;
			m_spawnPosition += m_player->GetForwardVector() * 20.0f;

			/* エフェクトを生成する。*/
			m_laserEffect = m_player->GetEffectList().PlayEffect(nsEffect::MagicAttack, m_spawnPosition, Quaternion::Identity, Vector3::One * 10.0f);
		}
	}
}