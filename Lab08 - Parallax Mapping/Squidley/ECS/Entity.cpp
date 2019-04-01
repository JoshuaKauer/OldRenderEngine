#include "Entity.h"

#include <string.h>
#include "Component.h"
#include "../Logger.h"

Entity::Entity()
{
	memset(m_components, 0, MAX_COMPONENTS * sizeof(m_components[0]));
}


Entity::~Entity()
{
}

bool Entity::Update(float dt)
{
	for (int j = 0; j < MAX_COMPONENTS; ++j)
	{
		if (m_components[j] && m_components[j]->IsEnabled())
		{
			bool result = m_components[j]->Update(dt);
			//assert(result); //log Error
			if (!result)
			{
				std::string tmp = (string)m_components[j]->GetName() + " Failed to Update";
				LOG(Severe, tmp.c_str());
			}
		}
	}
	for (int j = 0; j < MAX_COMPONENTS; ++j)
	{
		if (m_components[j] && m_components[j]->IsEnabled())
		{
			bool result = m_components[j]->Draw();
			//assert(result); //log Error
			if (!result)
			{
				std::string tmp = (string)m_components[j]->GetName() + " Failed to Draw";
				LOG(Severe, tmp.c_str());
			}
		}
	}
	return true;
}

void Entity::SetName(const char * const name)
{
	//strncpy_s(m_name, strlen(m_name), name, MAX_NAME_LEN);
	for (int j = 0; j < MAX_NAME_LEN; ++j)
	{
		m_name[j] = name[j];
		if (!name[j]) return;
	}
	m_name[MAX_NAME_LEN - 1] = 0;
}

bool Entity::AddComponent(Component * c, const char * const name)
{
	for (int j = 0; j < MAX_COMPONENTS; ++j)
	{
		if (!m_components[j])
		{
			m_components[j] = c;
			c->SetOwner(this);
			c->SetName(name);
			return true;
		}
	}
	//assert(false);
	std::string tmp = "Failed to add Component " + (string)name;
	LOG(Error, tmp.c_str());
	return false;
}

bool Entity::Initialize()
{
	for (int j = 0; j < MAX_COMPONENTS; ++j)
	{
		if (m_components[j])
		{
			if (!m_components[j]->Init()) return false;
		}
	}
	return true;
}
