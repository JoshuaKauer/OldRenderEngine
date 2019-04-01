#include "GravityComponent.h"
#include "SpatialComponent.h"
#include "MovementComponent.h"
#include "..\..\Ray.h"
#include "stdio.h"

#define REND_MAX 30

Renderable renders[REND_MAX];

short rendCount = 0;

bool GravityComponent::Update(float dt)
{
	//Ray::fireRay();
	MovementComponent* move = GetSiblingComponent<MovementComponent>();
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	//CameraComponent* camera = GetSiblingComponent<CameraComponent>();
	
	if (move)
	{
		float distancetoObject = m_distanceToGround + move->getSpeed() * dt + 1;

		float distance = Ray::fireRay(renders, rendCount, space->m_position + glm::vec3(0.0f, -m_distanceToGround + 1, 0.0f), -space->m_UP);

		if (distance < distancetoObject)
		{
			distancetoObject = distance;
		}

		//printf("%f\n", distance);

		if (distancetoObject < 1.0f)
		{
			//printf("Stop");
			move->moveUpInstant(1 - distancetoObject);
		}
		else if (distancetoObject < m_distanceToGround + move->getSpeed() * dt + 1)
		{
			move->moveDownInstant(distancetoObject - 1);
		}
		else
		{
			move->moveDown(dt);
		}
	}

	return true;
}

void GravityComponent::addRenderable(Renderable rend)
{
	if (rendCount < REND_MAX)
	{
		renders[rendCount++] = rend;
	}
}

//bool GravityComponent::Update(float dt)
//{
//	MovementComponent* movementComponent = GetSiblingComponent<MovementComponent>();
//	SpacialComponent* spacialComponent = GetSiblingComponent<SpacialComponent>();
//	CameraComponent* cameraComponent = GetSiblingComponent<CameraComponent>();
//
//	if (movementComponent)
//	{
//		Entity* entityArray = EntityManager::getEntityArray();
//
//		float distanceToNearestCollision = m_distanceToGround + movementComponent->GetSpeed() * dt + 1;
//
//		Renderable renderables[1000];
//
//		int numRenderables = 0;
//
//		ColliderComponent* colliderComponent = GetSiblingComponent<ColliderComponent>();
//
//		for (int j = 0; j < EntityManager::getNumEntities(); ++j)
//		{
//			if (entityArray[j].GetComponentByType<ColliderComponent>() != colliderComponent)
//			{
//				renderables[numRenderables++] = *entityArray[j].GetComponentByType<RenderableComponent>()->getRenderable();
//			}
//		}
//
//		float distanceToThisCollision = Rays::FireRay(spacialComponent->GetPosition() + glm::vec3(0.0f, -m_distanceToGround + 1, 0.0f), -spacialComponent->GetUp(), renderables, numRenderables);
//
//		if (distanceToThisCollision < distanceToNearestCollision)
//		{
//			distanceToNearestCollision = distanceToThisCollision;
//		}
//
//		if (distanceToNearestCollision < 1.0f)
//		{
//			movementComponent->MoveUpInstant(1 - distanceToNearestCollision);
//		}
//		else if (distanceToNearestCollision < m_distanceToGround + movementComponent->GetSpeed() * dt + 1)
//		{
//			movementComponent->MoveDownInstant(distanceToNearestCollision - 1);
//		}
//		else
//		{
//			movementComponent->MoveDown(dt);
//		}
//	}
//
//	return true;
//}