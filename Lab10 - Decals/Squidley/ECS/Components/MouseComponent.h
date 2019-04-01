#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "../Component.h"
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(pop)

class MouseComponent : public Component
{
public:
	glm::vec2 oldMousePosition;
	glm::vec3* lookAt;
	glm::vec3* strafeDirection;
	glm::vec3* UP;

	void mouseUpdate(const glm::vec2& newMousePosition);

	bool Initialize() override;
private:
	float ROTATIONAL_SPEED;// = ConfigReader::getFloatFromString(ConfigReader::findValueForKey("CameraRotateSpeed"));
};

