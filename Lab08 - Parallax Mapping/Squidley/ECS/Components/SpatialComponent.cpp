#include "SpatialComponent.h"
#include "../../../ObjConverter/ConfigReader.h"
#include "CameraComponent.h"

bool SpatialComponent::Initialize()
{
	return true;
}

bool SpatialComponent::Init(glm::vec3 lookAt, glm::vec3 UP, glm::vec3 strafe, glm::vec3 pos)
{
	m_lookAt = lookAt;
	m_UP = UP;
	m_strafeDirection = strafe;
	m_position = pos;
	return true;
}

bool SpatialComponent::Update(float dt)
{
	decelerate();

	if (velocity.length() > MAX_VELOCITY) velocity = glm::normalize(velocity) * MAX_VELOCITY;

	m_position += velocity * dt;
	//CameraComponent* cam = GetSiblingComponent<CameraComponent>();

	//if (cam == nullptr) m_lookAt = glm::normalize(velocity);
	//else printf("%f, %f, %f\n", m_position.x, m_position.y, m_position.z);

	//if(strcmp(GetName(), "Blue Way Spatial") == 0) printf("%f, %f, %f\n", m_position.x, m_position.y, m_position.z);
	
	return true;
}

glm::vec3* SpatialComponent::getUP()
{
	return &m_UP;
}

glm::vec3* SpatialComponent::getLookAt()
{
	return &m_lookAt;
}

glm::vec3* SpatialComponent::getPosition()
{
	return &m_position;
}

glm::vec3* SpatialComponent::getStrafeDirection()
{
	return &m_strafeDirection;
}

void SpatialComponent::accelerate(glm::vec3 accel)
{
	velocity += accel;
}

void SpatialComponent::decelerate()
{
	//velocity /= 5.0f;
	velocity = velocity / 1.1f;
}

