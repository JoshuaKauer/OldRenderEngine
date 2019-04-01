#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning(pop)

struct MyModel
{
	glm::vec3 lightPosition;
	glm::vec3 diffuseIntensity;
	glm::vec3 ambientIntensity;
	glm::vec3 objectColor;
	glm::vec3 specularColor;
	float specularExponent;
};