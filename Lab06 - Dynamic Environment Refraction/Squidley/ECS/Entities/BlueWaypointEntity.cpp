#include "BlueWaypointEntity.h"
#include "../Components/SpatialComponent.h"
#include "../Components/RenderableComponent.h"

void BlueWaypointEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3());
	Initialize();
}

void BlueWaypointEntity::setPosition(glm::vec3 pos)
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->m_position = pos;
	RenderableComponent* rend = GetComponentByType<RenderableComponent>();
	rend->makeRendered();
}