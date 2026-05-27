#include "stdafx.h"
#include "StateMachine.h"
#include "NullState.h"
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"

/* @def
 * @brief テンプレートのマクロ。
 */
#define CLASS_T template<class CharacterTemplete>

/* @def
 * @brief テンプレート名のマクロ。
 */
#define T_NAME CharacterTemplete

namespace nsApp
{
	namespace nsState
	{
		CLASS_T
	    StateMachine<T_NAME>::StateMachine(T_NAME* owner) : m_owner(owner)
		{
			/* nullステートの作成。*/
			m_currentState = new NullState<T_NAME>();

			/* nullステートの初期化。*/ 
			m_currentState->Register(m_owner, this);
		}


		CLASS_T
		StateMachine<T_NAME>::~StateMachine()
		{
			/* */
			if (m_currentState != nullptr)
			{
				/* ステートを終了する。*/
				m_currentState->Exit();
				/* ステートの削除。*/
				delete m_currentState;
			}
		}


		CLASS_T
	    void StateMachine<T_NAME>::ChangeState(IState<T_NAME>* newState)
		{
			if (m_currentState != nullptr)
			{
				/* 現在のステートを終了する。*/ 
				m_currentState->Exit();
				/* ステートを開放する。*/
				delete m_currentState;
			}

			/* 新しいステートをセットする。*/
			m_currentState = newState;
			/* ステートを登録する。*/
			m_currentState->Register(m_owner, this);
			/* 新しいステートを開始する。*/
			m_currentState->Enter();
		}


		CLASS_T
	    void StateMachine<T_NAME>::Update()
		{
			m_currentState->Update();
		}

		/* Actorクラスに対してテンプレートの使用可能にする。*/
		template class StateMachine<nsApp::nsActor::Actor>;

		/* NPC用ステートマシーンテンプレートを作成する。*/
		template class StateMachine<NPCBrain>;
	}
}