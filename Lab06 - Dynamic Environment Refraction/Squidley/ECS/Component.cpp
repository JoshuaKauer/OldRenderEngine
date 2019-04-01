#include "Component.h"
#include <stdio.h>

bool Component::s_breakable = false;

Component::Component()
{
	m_owner = 0;
}

bool Component::Init()
{
	m_enabled = true;

	bool result = Initialize();
	if (!result) printf("Component failed to Initialize\n");
	else printf("Component Initialized successfully\n");
	return result;
}

void Component::SetName(const char* const name)
{
	for (int j = 0; j < MAX_NAME_LEN; ++j)
	{
		m_name[j] = name[j];
		if (!name[j]) return;
	}
	m_name[MAX_NAME_LEN - 1] = 0;
}

void Component::SetBreak(bool enabled)
{
	s_breakable = enabled;
}

void Component::Break(bool condition, bool keepBreakable)
{
	if (condition && s_breakable)
	{
		__debugbreak();
		s_breakable = keepBreakable;
	}
}

void Component::BreakIf(bool condition)
{
	if (condition)
	{
		__debugbreak();
	}
}
