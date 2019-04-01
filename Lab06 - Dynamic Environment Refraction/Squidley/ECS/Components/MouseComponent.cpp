#include "MouseComponent.h"
#include "..\..\..\ObjConverter\ConfigReader.h"
#include <glm/gtx/transform.hpp>
#include "SpatialComponent.h"

void MouseComponent::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	*strafeDirection = glm::cross(*lookAt, *UP);

	*lookAt = glm::mat3(
		glm::rotate(mouseDelta.x * ROTATIONAL_SPEED, *UP) *
		glm::rotate(mouseDelta.y * ROTATIONAL_SPEED, *strafeDirection)) * *lookAt;

	oldMousePosition = newMousePosition;
}

bool MouseComponent::Initialize()
{
	ROTATIONAL_SPEED = ConfigReader::getFloatFromString(ConfigReader::findValueForKey("CameraRotateSpeed"));
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	lookAt = space->getLookAt();
	UP = space->getUP();
	strafeDirection = space->getStrafeDirection();
	return true;
}
