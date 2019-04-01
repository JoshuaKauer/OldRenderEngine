#include "PhaserComponent.h"
#include "../../Ray.h"
#include "CameraComponent.h"

#define REND_MAX 30

Renderable renderable[REND_MAX];

short renderableCount = 0;

glm::vec3 PhaserComponent::fire()
{
	CameraComponent* camera = GetSiblingComponent<CameraComponent>();
	float distance = Ray::fireRay(renderable, renderableCount, camera->position, camera->viewDirection);
	return camera->viewDirection * distance + camera->position;
}

void PhaserComponent::addRenderable(Renderable rend)
{
	if (renderableCount < REND_MAX)
	{
		renderable[renderableCount++] = rend;
	}
}