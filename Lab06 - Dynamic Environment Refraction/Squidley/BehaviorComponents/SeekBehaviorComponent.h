#pragma once
#include "SteeringBehaviors.h"

class SeekBehavior : public SteeringBehaviors
{
public:
	glm::vec3* targetPos;

	bool Update(float) override;

	void setTargetPos(glm::vec3*);
};