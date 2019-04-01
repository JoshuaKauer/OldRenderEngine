#include "FlagEntity.h"
#include "../Components/SpatialComponent.h"

void FlagEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3());
	Initialize();
}

void FlagEntity::setPosition(glm::vec3 pos)
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->m_position = pos;
}