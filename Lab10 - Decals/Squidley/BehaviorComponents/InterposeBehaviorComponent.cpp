#include "InterposeBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"

bool InterposeBehavior::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(interpose(space->m_position, *targetPos, *targetVel, *target2Pos, *target2Vel, space->velocity, MAX_VELOCITY, MAX_ROTATE));// *dt);
	return true;
}

void InterposeBehavior::setTargetPos(glm::vec3* target)
{
	targetPos = target;
}

void InterposeBehavior::setTarget2Pos(glm::vec3* target)
{
	target2Pos = target;
}

void InterposeBehavior::setTargetVel(glm::vec3* target)
{
	targetVel = target;
}

void InterposeBehavior::setTarget2Vel(glm::vec3* target)
{
	target2Vel = target;
}
