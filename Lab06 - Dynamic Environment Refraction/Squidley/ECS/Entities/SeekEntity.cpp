#include "SeekEntity.h"
#include "../Components/SpatialComponent.h"

void SeekEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3(15.0f, 12.0f, 60.0f));
	Initialize();
}
