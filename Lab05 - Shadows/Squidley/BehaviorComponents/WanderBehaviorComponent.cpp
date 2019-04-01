#include "WanderBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool WanderBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(wander(space->m_position, space->velocity, circleDist, circleRad));
	return true;
}

void WanderBehavior::setCircleDist(float dist)
{
	circleDist = dist;
}

void WanderBehavior::setCircleRad(float rad)
{
	circleRad = rad;
}

void WanderBehavior::setVel(glm::vec3 vel)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->velocity = vel;
}
