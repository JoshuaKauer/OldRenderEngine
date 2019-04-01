#ifndef SHAPEDATA_H
#define SHAPEDATA_H
#include <GL\glew.h>
#include "Vertex.h"

struct ShapeData
{
	ShapeData() : vertices(0), numVertices(0), indices(0), numIndices(0){}
	Vertex* vertices;
	GLuint numVertices;
	GLuint* indices;
	GLuint numIndices;
	GLenum glEnums;
	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLuint);
	}
	void cleanup()
	{
		delete[] indices;
		delete[] vertices;
		numVertices = numIndices = 0;
		//glEnums = NULL;
	}
};

#endif