#include "PathEntity.h"
#include "../Components/SpatialComponent.h"

void PathEntity::Init(glm::vec3 start)
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	if (space)
	{
		space->Init(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), start);
		Initialize();
	}
}
