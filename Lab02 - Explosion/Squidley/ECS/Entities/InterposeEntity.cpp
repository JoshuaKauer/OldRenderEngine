#include "InterposeEntity.h"

#include "../Components/SpatialComponent.h"

void InterposeEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3(0.0f, 20.0f, 0.0f));
	Initialize();
}