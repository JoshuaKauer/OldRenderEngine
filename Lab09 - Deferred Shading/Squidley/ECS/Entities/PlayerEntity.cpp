#include "PlayerEntity.h"
#include "../../../ObjConverter/ConfigReader.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
#pragma warning(pop)
#include "../Components/SpatialComponent.h"

bool PlayerEntity::Init()
{
	ConfigReader::initialize();
	//MOVEMENT_SPEED = ConfigReader::getFloatFromString(ConfigReader::findValueForKey("CameraMoveSpeed"));
	//ROTATIONAL_SPEED = ConfigReader::getFloatFromString(ConfigReader::findValueForKey("CameraRotateSpeed"));
	std::string cameraPos = ConfigReader::findValueForKey("InitialCameraPosition");
	glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	float pos[3];
	ConfigReader::getFloatsFromString(cameraPos.c_str(), pos, 3);
	glm::vec3 position = glm::vec3(pos[0], pos[1], pos[2]);
	cameraPos = ConfigReader::findValueForKey("InitialCameraDirection");
	ConfigReader::getFloatsFromString(cameraPos.c_str(), pos, 3);
	glm::vec3 lookAt = glm::vec3(pos[0], pos[1], pos[2]);
	glm::vec3 strafeDirection = glm::vec3(glm::cross(lookAt, UP));
	SpatialComponent* space = GetComponentByType<SpatialComponent>();
	space->Init(lookAt, UP, strafeDirection, position);
	Initialize();
	return true;
}
