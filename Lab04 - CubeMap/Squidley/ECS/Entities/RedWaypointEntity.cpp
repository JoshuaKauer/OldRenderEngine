#include "RedWaypointEntity.h"
#include "../Components/SpatialComponent.h"
#include "../Components/RenderableComponent.h"

void RedWaypointEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3());
	Initialize();
}

void RedWaypointEntity::setPosition(glm::vec3 pos)
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->m_position = pos;
	RenderableComponent* rend = GetComponentByType<RenderableComponent>();
	rend->makeRendered();
}
