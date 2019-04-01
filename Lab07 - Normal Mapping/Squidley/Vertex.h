#ifndef VERTEX_H
#define VERTEX_H
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
#pragma warning(pop)

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};

#endif