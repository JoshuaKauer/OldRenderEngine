#ifndef CAMERA
#define CAMERA
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning (pop)
#include "../Component.h"

class CameraComponent : public Component
{
	
public:
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 UP;

	bool Initialize() override;
	bool Update(float) override;
	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3 getPosition();
};

#endif
