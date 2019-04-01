#pragma once
#include "SteeringBehaviors.h"

class WanderBehavior : public SteeringBehaviors
{
public:
	float circleDist;
	float circleRad;

	bool Update(float) override;

	void setCircleDist(float);
	void setCircleRad(float);
	void setVel(glm::vec3);
};
