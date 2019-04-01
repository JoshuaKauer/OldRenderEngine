#include "CameraComponent.h"
#include <glm\gtx\transform.hpp>
#include "SpatialComponent.h"

bool CameraComponent::Initialize()
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	position = *space->getPosition();
	viewDirection = *space->getLookAt();
	UP = *space->getUP();
	return true;
}

bool CameraComponent::Update(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	position = *space->getPosition();
	viewDirection = *space->getLookAt();
	UP = *space->getUP();
	return true;
}

glm::mat4 CameraComponent::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

glm::vec3 CameraComponent::getPosition()
{
	return position;
}
