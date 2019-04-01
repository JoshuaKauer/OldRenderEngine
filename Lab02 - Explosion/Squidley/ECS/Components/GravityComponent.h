#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "../Component.h"

class Renderable;

class GravityComponent : public Component
{
public:
	float m_distanceToGround = 0.0f;
	bool Update(float) override;
	void addRenderable(Renderable);
};
