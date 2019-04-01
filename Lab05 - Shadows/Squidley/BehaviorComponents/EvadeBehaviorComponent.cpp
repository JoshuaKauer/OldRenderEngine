#include "EvadeBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool EvadeBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(evade(space->m_position, *targetPos, *targetVel, space->velocity, MAX_VELOCITY, MAX_ROTATE));// *dt);
	return true;
}

void EvadeBehavior::setTargetPos(glm::vec3* target)
{
	targetPos = target;
}

void EvadeBehavior::setTargetVel(glm::vec3* target)
{
	targetVel = target;
}
