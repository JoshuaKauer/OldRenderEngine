#pragma once
#include "SteeringBehaviors.h"

class InterposeBehavior : public SteeringBehaviors
{
public:
	glm::vec3* targetPos;
	glm::vec3* targetVel;
	glm::vec3* target2Pos;
	glm::vec3* target2Vel;

	bool Update(float) override;

	void setTargetPos(glm::vec3*);
	void setTarget2Pos(glm::vec3*);
	void setTargetVel(glm::vec3*);
	void setTarget2Vel(glm::vec3*);
};