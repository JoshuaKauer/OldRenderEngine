#include "RenderableComponent.h"
#include "glm/gtx/vector_angle.hpp"

Renderable* RenderableComponent::getRenderable()
{
	return mesh;
}

void RenderableComponent::setRenderable(Renderable* rend)
{
	mesh = rend;
}

void RenderableComponent::makeRendered()
{
	mesh->m_isRendered = true;
}

void RenderableComponent::makeNotRendered()
{
	mesh->m_isRendered = false;
}

bool RenderableComponent::isRendered()
{
	return mesh->m_isRendered;
}

bool RenderableComponent::Update(float)
{
	space = GetSiblingComponent<SpatialComponent>();
	mesh->m_transformInfo.m_translateTransform = *space->getPosition();
	if (space->m_lookAt.z >= 0)
		mesh->m_transformInfo.rotatey = -glm::angle(glm::vec2(space->m_lookAt.x, space->m_lookAt.z), glm::vec2(1.0f, 0.0f));
	else
		mesh->m_transformInfo.rotatey = glm::angle(glm::vec2(space->m_lookAt.x, space->m_lookAt.z), glm::vec2(1.0f, 0.0f));
	return true;
}
