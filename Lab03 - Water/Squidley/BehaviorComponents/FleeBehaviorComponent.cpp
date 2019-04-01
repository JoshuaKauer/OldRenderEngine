#include "FleeBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool FleeBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(flee(space->m_position, *targetPos, space->velocity, MAX_VELOCITY, MAX_ROTATE));// *dt);
	return true;
}

void FleeBehavior::setTargetPos(glm::vec3* target)
{
	targetPos = target;
}
