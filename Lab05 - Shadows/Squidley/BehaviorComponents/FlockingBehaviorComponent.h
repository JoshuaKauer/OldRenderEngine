#pragma once

#include "SteeringBehaviors.h"

#define NUM_FLOCKERS 25

class FlockingBehavior : public SteeringBehaviors
{
public:
	glm::vec3* flockPos;
	glm::vec3* flockVel;
	Entity* flock;
	int numFlockers;

	bool Update(float) override;

	void setFlock(Entity*);
	void setVel(glm::vec3);
	void setNumFlockers(int);
};