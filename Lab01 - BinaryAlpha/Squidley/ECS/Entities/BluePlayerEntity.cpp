#include "BluePlayerEntity.h"
#include "../Components/SpatialComponent.h"

void BluePlayerEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3(-74.0f, 17.0f, 166.0f));
	Initialize();
}
