#pragma once
#include "SteeringBehaviors.h"

class EvadeBehavior : public SteeringBehaviors
{
public:
	glm::vec3* targetPos;
	glm::vec3* targetVel;

	bool Update(float) override;

	void setTargetPos(glm::vec3*);
	void setTargetVel(glm::vec3*);
};