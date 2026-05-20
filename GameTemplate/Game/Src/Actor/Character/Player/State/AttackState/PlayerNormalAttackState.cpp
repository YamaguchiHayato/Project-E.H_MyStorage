#include "stdafx.h"
#include "PlayerNormalAttackState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerNormalAttackState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			m_currentAttackType = AttackType::NormalAttack;

			/* 攻撃アニメーションを再生する。*/
			m_player->PlayWeaponAnimation(AttackType::NormalAttack);
		}


		void PlayerNormalAttackState::OnAttackTick()
		{
			/* 1フレーム目で近接武器の当たり判定をオンにする。*/
			if (m_attackTimer == 1)
				m_player->GetWeaponHitDetection().Enable();

			/* 48フレーム目で遠距離武器の弾丸を発射。*/
			if (m_attackTimer == 48)
			{
				/* 杖の場合。*/
				if (m_player->GetCurrentWeapon() == WeaponType::Wand)
					SummonMissile();

				/* 銃の場合。*/
				else if (m_player->GetCurrentWeapon() == WeaponType::TwinGun)
					FireGunBullet();
			}
		}


		bool PlayerNormalAttackState::OnUpdateAttack()
		{
			/* 48フレーム目に弾丸を生成する前に5フレームウェイトを挟む。*/
			if ((m_player->GetCurrentWeapon() == WeaponType::Wand || m_player->GetCurrentWeapon() == WeaponType::TwinGun) && m_attackTimer < 50)
				return true;

			return false;
		}


		bool PlayerNormalAttackState::OnRequestAttackID(uint8_t& id)
		{
			return CheckCombo(nsActor::PlayerStateID::enNormalAttack, id);
		}


		void PlayerNormalAttackState::SummonMissile()
		{
			/* 魔法の生成。*/
			if (m_player->GetCurrentWeapon() == WeaponType::Wand)
				ConstructAndTransmitMagicRequest(nsActor::MagicType::enNormalMagic);
		}


		void PlayerNormalAttackState::FireGunBullet()
		{
			/* 座標を取得。*/
			m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();

			/* 前方向のベクトルを取得する。*/
			m_forwardDirection = m_player->GetForwardVector();

			/* 生成する弾丸の種類を設定する。*/
			ConstructAndTransmitBulletRequest(BulletType::enNormal);
		}
	}
}