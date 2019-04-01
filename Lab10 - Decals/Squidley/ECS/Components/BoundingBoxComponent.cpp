#include "BoundingBoxComponent.h"
#include "SpatialComponent.h"

bool BoundingBoxComponent::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();

	if (space->m_position.x < min.x)
	{
		space->m_position.x = min.x;
		space->velocity.x *= -1;
	}
	else if (space->m_position.x > max.x)
	{
		space->m_position.x = max.x;
		space->velocity.x *= -1;
	}

	if (space->m_position.y < min.y)
	{
		space->m_position.y = min.y;
		space->velocity.y *= -1;
	}
	else if (space->m_position.y > max.y)
	{
		space->m_position.y = max.y;
		space->velocity.y *= -1;
	}

	if (space->m_position.z < min.z)
	{
		space->m_position.z = min.z;
		space->velocity.z *= -1;
	}
	else if (space->m_position.z > max.z)
	{
		space->m_position.z = max.z;
		space->velocity.z *= -1;
	}

	return true;
}

void BoundingBoxComponent::setMin(glm::vec3 box)
{
	min = box;
}

void BoundingBoxComponent::setMax(glm::vec3 box)
{
	max = box;
}
