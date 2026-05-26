#include "stdafx.h"
#include "NPCHelpState.h"

#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "ResourceUtility.h"

namespace nsApp
{
	namespace nsState
	{
		NPCHelpState::NPCHelpState(nsActor::Player* helpTarget)
			: m_helpTarget(helpTarget)
		{
		}


		void NPCHelpState::Enter()
		{
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
				return;

			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			if (m_vInput != nullptr)
				m_vInput->Reset();

			m_helpTarget = nsActor::ResourceUtility::FindPlayerByTag(m_helpTargetTag);

			if (nsActor::ResourceUtility::IsValidRescueTarget(m_body, m_helpTarget))
				m_helpTarget->GetRescueStatusManager().SetBeingHelped(true);

			else
				m_helpTarget = nullptr;
		}

		
		void NPCHelpState::Update()
		{
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			m_helpTarget = nsActor::ResourceUtility::FindPlayerByTag(m_helpTargetTag);

			if (!nsActor::ResourceUtility::IsValidRescueTarget(m_body, m_helpTarget))
			{
				m_vInput->Reset();

				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}

			m_difference = m_helpTarget->GetPosition() - m_body->GetPosition();
			m_difference.y = 0.0f;

			m_distance = m_difference.Length();

			if (m_distance > m_helpRange)
			{
				MoveToHelpTarget();
				return;
			}

			ExecuteHelp();
		}

		void NPCHelpState::Exit()
		{
			if (m_vInput != nullptr)
				m_vInput->Reset();

			// ‚Ü‚¾Ž€–S’†‚Ì‚Ü‚Ü‹~•ó‘Ô‚ð”²‚¯‚é‚È‚çA—\–ñ‚ð‰ðœ‚·‚é
			auto* target = nsActor::ResourceUtility::FindPlayerByTag(m_helpTargetTag);

			if (target != nullptr && target != m_body)
				target->GetRescueStatusManager().SetBeingHelped(false);

			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
			m_helpTarget = nullptr;
			m_difference = Vector3::Zero;
			m_distance = 0.0f;
		}


		void NPCHelpState::MoveToHelpTarget()
		{
			if (m_vInput == nullptr)
				return;

			if (m_distance <= 0.001f)
			{
				m_vInput->SetLStick(0.0f, 0.0f);
				return;
			}

			m_difference.Normalize();

			m_vInput->SetLStick(m_difference.x, m_difference.z);
		}


		void NPCHelpState::ExecuteHelp()
		{
			if (m_helpTarget == nullptr || m_vInput == nullptr)
				return;

			// ‹~•‘ÎÛ‚ð•œŠˆ‚³‚¹‚éB
			m_vInput->SetLStick(0.0f, 0.0f);
			m_vInput->RequestButton(enButtonY, 3);
		}


		bool NPCHelpState::IsValidHelpTarget() const
		{
			if (m_helpTarget == nullptr)
				return false;

			if(m_helpTarget == m_body)
				return false;

			return m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0;
		}
	}
}