#include "stdafx.h"
#include "NPCBrain.h"
#include "Boss.h"

#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace nsApp
{
	void NPCBrain::Init(nsActor::Player* outer)
	{
		m_outer = outer;
		/* NPC用のステートマシーンの生成。*/
		m_npcStateMachine = new nsState::StateMachine<NPCBrain>(this);

		/* NPC用ステートクラスを生成。*/
		m_npcStateMachine->ChangeState(new nsState::NPCIdleState());
	}


	void NPCBrain::Update()
	{
		/* 早期リターン。*/
		if (m_outer == nullptr || m_virtualInputAdapter == nullptr)
			return;

		/* ターゲットを探す。*/
		m_helpTarget = m_outer->SearchCharacter();

		/* ターゲットが居ない場合は何もしない。*/
		if (m_npcStateMachine != nullptr)
			m_npcStateMachine->Update();
	}


	nsActor::ICharacter* NPCBrain::SearchTarget()
	{
		if(m_bossTarget == nullptr)
		  /* 目標を探索する。*/
		  m_bossTarget = FindGO<nsActor::Boss>("boss");

		/* 見つからなかった場合。*/
		if (m_bossTarget == nullptr || reinterpret_cast<uintptr_t>(m_bossTarget) == 0xFFFFFFFFFFFFFFFF)
			return nullptr;

		/* BossのHPが0になった場合。*/
		if(m_bossTarget->GetCharacterStatus().hp.currentHP <= 0)
			return nullptr;

		return m_bossTarget;
	}
}