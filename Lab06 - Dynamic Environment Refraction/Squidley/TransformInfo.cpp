#include "TransformInfo.h"
#include "glm/gtx/transform.hpp"

glm::mat4 TransformInfo::getFinalTransform()
{
	glm::mat4 modelToWorld;
	if (m_translateTransform != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		modelToWorld *= glm::translate(m_translateTransform);
	}
	//if (m_rotateTransform != glm::vec3(0.0f, 0.0f, 0.0f))
	//{
	//	modelToWorld *= glm::rotate(rotate, m_rotateTransform);
	//}
	if (rotatex != 0.0f)
	{
		if (isRotating)
		{
			rotatex += theta;
		}
		modelToWorld *= glm::rotate(rotatex, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (rotatey != 0.0f)
	{
		modelToWorld *= glm::rotate(rotatey, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (rotatez != 0.0f)
	{
		modelToWorld *= glm::rotate(rotatez, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (m_scaleTransform != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		modelToWorld *= glm::scale(m_scaleTransform);
	}
	return modelToWorld;
}
