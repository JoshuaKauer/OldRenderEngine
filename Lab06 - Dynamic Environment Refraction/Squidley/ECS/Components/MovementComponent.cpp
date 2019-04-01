#include "MovementComponent.h"
#include "../../../ObjConverter/ConfigReader.h"
#include "SpatialComponent.h"

bool MovementComponent::Update(float dt)
{
	dt;
	return true;
}

bool MovementComponent::Initialize()
{
	moveSpeed = ConfigReader::getFloatFromString(ConfigReader::findValueForKey("CameraMoveSpeed"));
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	m_position = space->getPosition();
	m_UP = space->getUP();
	m_lookAt = space->getLookAt();
	m_strafeDirection = space->getStrafeDirection();
	return true;
}

void MovementComponent::moveForward(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(*m_lookAt * 2.5f);
}

void MovementComponent::moveBackward(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(*m_lookAt * -2.5f);
}

void MovementComponent::strafeLeft(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(*m_strafeDirection * -2.5f);
}

void MovementComponent::strafeRight(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(*m_strafeDirection * 2.5f);
}

void MovementComponent::moveUp(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(*m_UP * 2.5f);
}

void MovementComponent::moveDown(float)
{
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	space->accelerate(*m_UP * -2.5f);
}

float MovementComponent::getSpeed()
{
	return moveSpeed;
}

void MovementComponent::moveUpInstant(float change)
{
	m_position->y += change;
	//*m_position += moveSpeed * *m_UP * change;
}

void MovementComponent::moveDownInstant(float change)
{
	m_position->y -= change;
	//*m_position -= moveSpeed * *m_UP * change;
}
