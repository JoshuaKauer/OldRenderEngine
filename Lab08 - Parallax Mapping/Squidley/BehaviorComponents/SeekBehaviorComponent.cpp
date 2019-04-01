#include "SeekBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool SeekBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(seek(space->m_position, *targetPos, space->velocity, MAX_VELOCITY, MAX_ROTATE));// *dt);
	return true;
}

void SeekBehavior::setTargetPos(glm::vec3* target)
{
	targetPos = target;
}
