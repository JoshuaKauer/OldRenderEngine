#pragma once
#include "../Entity.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
#pragma warning(pop)

class BlueWaypointEntity : public Entity
{
public:
	void Init();
	void setPosition(glm::vec3);
};
