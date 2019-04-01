#pragma once
#include <GL\glew.h>
#include "Vertex.h"

struct Scene
{
	int vertexFormat;
	int numVertices;
	int numIndices;
	int sizeVertex;
	int sizeIndex;
	void* vertices;
	void* indices;
};