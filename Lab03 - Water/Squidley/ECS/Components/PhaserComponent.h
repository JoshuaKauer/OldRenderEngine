#pragma once
#include "../Component.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)
class Renderable;

class PhaserComponent : public Component
{
public:
	glm::vec3 fire();
	void addRenderable(Renderable);
};

