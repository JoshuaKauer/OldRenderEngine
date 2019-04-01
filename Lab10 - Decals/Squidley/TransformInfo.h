#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm\glm.hpp"
#pragma  warning(pop)
class TransformInfo
{
	
public:
	glm::vec3 m_scaleTransform;
	glm::vec3 m_rotateTransform;
	glm::vec3 m_translateTransform;
	glm::vec3 m_worldToProjectionMatrix;
	bool isRotating = false;
	float theta = 0.0f;
	float rotate;
	float rotatex;
	float rotatey;
	float rotatez;

	glm::mat4 getFinalTransform();
};

