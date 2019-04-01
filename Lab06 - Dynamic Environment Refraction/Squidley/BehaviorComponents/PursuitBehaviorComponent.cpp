#include "PursuitBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool PursuitBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(pursuit(space->m_position, *targetPos, *targetVel, space->velocity, MAX_VELOCITY, MAX_ROTATE));// *dt);
	return true;
}

void PursuitBehavior::setTargetPos(glm::vec3* target)
{
	targetPos = target;
}

void PursuitBehavior::setTargetVel(glm::vec3* target)
{
	targetVel = target;
}
