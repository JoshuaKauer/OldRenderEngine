#pragma once
#include "../ECS/Entity.h"

class State
    {
    public:
        State(const char* const name) : m_name(name) {}
        virtual ~State(void)    {}

        virtual bool OnEnter (Entity*) {}   // log here
        virtual bool OnExit  (Entity*) {}   // log here
        virtual bool Execute (Entity*) = 0; // must implement

        const char* GetName () const { return m_name; }

    protected:
        const char* const m_name;

    private:
        State& operator=(const State&); // can't accidently copy states
    };
