#include "stdafx.h"
#include "PlayerDethState.h"

namespace
{
	const auto MOVE_FRAME_RATE = 1.0f / 60.0f; //! 1フレームあたりの固定時間。
	const auto FALLINGSPEED = 30.0f;           //! 落下速度。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerDethState::Enter()
		{
			/* キャスト。*/ 
			m_player = static_cast<nsActor::Player*>(m_owner);
			if (m_player == nullptr)
				return;

			/* アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Death);
		}


		void PlayerDethState::Update()
		{
			/* 早期リターン。*/
			if (m_player == nullptr)
				return;
		}


		bool PlayerDethState::RequestID(uint8_t& id)
		{
			return false;
		}
	}
}