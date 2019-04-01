#pragma once
#include "..\Component.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#pragma warning(disable : 4458)
#include "glm/glm.hpp"
#pragma warning(pop)

class BoundingBoxComponent : public Component
{
public:
	glm::vec3 min, max;

	bool Update(float) override;

	void setMin(glm::vec3);
	void setMax(glm::vec3);
};
