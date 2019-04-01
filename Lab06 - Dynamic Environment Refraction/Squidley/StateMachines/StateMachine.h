#pragma once
#include "State.h"
#include <cassert>

class StateMachine
    {
    public:
        StateMachine(Entity* owner=0) : m_owner(owner), m_currentState(0) {}
        virtual ~StateMachine(void) {}

        void     CurrentState (State* state) { m_currentState  = state; }
        State* CurrentState () const { return m_currentState;  }

        void Init (Entity* owner, State* state) 
{ assert(owner); assert(state); m_owner = owner; m_currentState = state; }

        void Update (float /*deltaTime*/)
            {
            assert(m_owner);
            if (m_currentState) m_currentState->Execute(m_owner);
            }

        void ChangeState(State* newState)
            {
            assert (newState);
            if (m_currentState) m_currentState->OnExit(m_owner);
            m_currentState = newState;
            m_currentState->OnEnter (m_owner);
            }

    private:
        Entity*  m_owner;
        State* m_currentState;
    };
