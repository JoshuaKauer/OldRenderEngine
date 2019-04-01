#include "FlockEntity.h"
#include "../Components/SpatialComponent.h"
#include "../../Random.h"

void FlockEntity::Init()
{
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(Random::randomInRange(-82.0f, 34.0f), Random::randomInRange(0.0f, 20.0f), Random::randomInRange(37.0f, 87.0f)));
	Initialize();
}
