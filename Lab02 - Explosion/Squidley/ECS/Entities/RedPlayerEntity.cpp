#include "RedPlayerEntity.h"
#include "../Components/SpatialComponent.h"

void RedPlayerEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3(15.0f, 17.5f, 12.0f));
	Initialize();
}
