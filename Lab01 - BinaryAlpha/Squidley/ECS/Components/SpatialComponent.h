#pragma once
#include "../Component.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
#pragma warning(pop)

#define MAX_VELOCITY 10.0f
#define MAX_ROTATE 5.0f

class SpatialComponent : public Component
{
public:
	bool Initialize() override;
	bool Init(glm::vec3 lookAt, glm::vec3 UP, glm::vec3 strafe, glm::vec3 pos);
	bool Update(float);

	glm::vec3* getUP();
	glm::vec3* getLookAt();
	glm::vec3* getPosition();
	glm::vec3* getStrafeDirection();
	void accelerate(glm::vec3);
	void decelerate();

	glm::vec3 m_lookAt;
	glm::vec3 m_UP;
	glm::vec3 m_strafeDirection;
	glm::vec3 m_position;
	glm::vec3 velocity;

	float maxSpeed;

	
};

