#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtx/transform.hpp>
#pragma warning(pop)
#include "../Component.h"

class MovementComponent : public Component
{
private:
	glm::vec3* m_UP;
	glm::vec3* m_position;
	glm::vec3* m_lookAt;
	glm::vec3* m_strafeDirection;

	
public:
	float moveSpeed;
	virtual bool Update(float dt);
	virtual bool Initialize();
	void moveForward(float);
	void moveBackward(float);
	void strafeLeft(float);
	void strafeRight(float);
	void moveUp(float);
	void moveDown(float);
	float getSpeed();
	void moveUpInstant(float);
	void moveDownInstant(float);
};
