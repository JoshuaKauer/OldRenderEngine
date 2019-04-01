#include "ArrivalBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool ArrivalBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(arrival(space->m_position, *targetPos, space->velocity, MAX_VELOCITY, MAX_ROTATE, 10.0f));// *dt);
	return true;
}

void ArrivalBehavior::setTargetPos(glm::vec3* target)
{
	targetPos = target;
}
