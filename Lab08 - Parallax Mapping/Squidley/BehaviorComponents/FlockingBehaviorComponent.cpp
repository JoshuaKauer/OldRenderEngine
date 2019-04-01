#include "FlockingBehaviorComponent.h"
#include "../ECS/Components/SpatialComponent.h"



bool FlockingBehavior::Update(float)
{
	glm::vec3 pos[NUM_FLOCKERS];
	glm::vec3 vel[NUM_FLOCKERS];

	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();

	for (int j = 0; j < NUM_FLOCKERS; ++j)
	{
		SpatialComponent* fSpace = flock[j].GetComponentByType<SpatialComponent>();
		pos[j] = fSpace->m_position;
		vel[j] = fSpace->velocity;
	}

	space->accelerate(flocking(pos, vel, numFlockers, space->m_position, space->velocity, MAX_VELOCITY, MAX_ROTATE, 0.1f, 0.7f, 0.69f));

	return true;
}
void FlockingBehavior::setFlock(Entity* theFlock)
{
	flock = theFlock;
}

void FlockingBehavior::setVel(glm::vec3 vel)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->velocity = vel;
}

void FlockingBehavior::setNumFlockers(int numFlock)
{
	numFlockers = numFlock;
}
