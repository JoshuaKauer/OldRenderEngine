#include "ShapeGenerator.h"
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning (pop)
#include <iostream>
#include "Vertex.h"
#include "RenderEngine/RenderEngine.h"
#include "Scene.h"
#include "Reader.h"
#include "VertexFormats.h"
#include "Logger.h"

#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

Geometry ShapeGenerator::geo[MAX_GEOMETRIES];
unsigned ShapeGenerator::numGeo = 0;

Geometry fakeGeo;

enum SphereDefines
{
	NUMSPHEREITERATIONS = 4,  // if you change this, you must change the next one too!!!
	FOUR_TO_NUM_ITERATIONS = 256,     // 4^numIterations, ie, 4 to the power of
	NUMSPHEREFACETS = FOUR_TO_NUM_ITERATIONS * 8, // 4^numIterations * 8
	NUMSPHEREVERTS = 3 * NUMSPHEREFACETS, // 3 verts per facet/triangle
	NUMSPHEREINDICES = 2 * NUMSPHEREVERTS   // two indices per point
};

Vertex sphereVerts[NUMSPHEREVERTS];
Facet3      sphereFacets[NUMSPHEREFACETS];
GLuint    sphereIndices[NUMSPHEREINDICES];

glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

Geometry* ShapeGenerator::makeScene(char* fileName)
{
	if (numGeo < MAX_GEOMETRIES)
	{
		Reader read;

		++numGeo;

		Scene* shape = read.readSceneFile(fileName);

		geo[numGeo].vertices = shape->vertices;
		geo[numGeo].indices = shape->indices;

		geo[numGeo].vertexFormat = shape->vertexFormat;
		geo[numGeo].m_vertexCount = shape->numVertices;
		//geo.m_vertexSize = geo.m_vertexCount * sizeof(Vertex);
		geo[numGeo].m_vertexSize = shape->sizeVertex * shape->numVertices;
		//ret.vertices = new Vertex[ret.numVertices];
		geo[numGeo].m_vertexStride = shape->sizeVertex;

		geo[numGeo].m_indexCount = shape->numIndices;
		//geo.m_indexSize = geo.m_indexCount * sizeof(GLuint);
		geo[numGeo].m_indexSize = shape->sizeIndex * shape->numIndices;
		//ret.indices = new GLuint[ret.numIndices];
		geo[numGeo].m_indexStride = sizeof(GLuint);
		geo[numGeo].glEnums = GL_TRIANGLES;
		//printf("%i %i\n", geo[numGeo].m_vertexSize, geo[numGeo].m_indexSize);
		RenderEngine::AddGeometry(shape->vertices, shape->indices, geo[numGeo]);

		return &geo[numGeo];
	}
	else
	{
		LOG(Severe, "Max number of geometries reached");
		return &fakeGeo;
	}
}

Geometry* ShapeGenerator::makeLine(glm::vec3 point1, glm::vec3 point2)
{
	if (numGeo < MAX_GEOMETRIES)
	{
		++numGeo;

		PColor verts[] =
		{
			point1,
			glm::vec3(1.0f, 0.0f, 0.0f),

			point2,
			glm::vec3(1.0f, 0.0f, 0.0f),
		};

		GLuint indices[] = { 0, 1 };

		geo[numGeo].vertices = verts;
		geo[numGeo].indices = indices;

		geo[numGeo].vertexFormat = PositionColor;
		geo[numGeo].m_vertexCount = 2;
		geo[numGeo].m_vertexSize = sizeof(PColor) * 2;
		geo[numGeo].m_vertexStride = sizeof(PColor);

		geo[numGeo].m_indexCount = 2;
		geo[numGeo].m_indexSize = sizeof(GLuint) * 2;
		geo[numGeo].m_indexStride = sizeof(GLuint);
		geo[numGeo].glEnums = GL_LINES;
		RenderEngine::AddGeometry(verts, indices, geo[numGeo]);

		return &geo[numGeo];
	}

	LOG(Severe, "Max number of geometries reached");
	return &fakeGeo;
}

ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData ret;

	Vertex myTri[] =
	{
		glm::vec3(+0.0f, +0.05f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.5f),
		
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(+0.05f, -0.07f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
		
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(-0.05f, -0.07f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),
		
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));
	
	GLuint indices[] = { 0, 1, 2 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_TRIANGLES;

	return ret;
}

ShapeData ShapeGenerator::makeCube()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f), // 0
		glm::vec3(+1.0f, +0.0f, +0.0f), // Color
		
		glm::vec3(+0.0f, +1.0f, +0.0f), //Normal
		glm::vec3(+1.0f, +1.0f, +1.0f), // 1
		glm::vec3(+0.0f, +1.0f, +0.0f), // Color
		
		glm::vec3(+0.0f, +1.0f, +0.0f), //Normal
		glm::vec3(+1.0f, +1.0f, -1.0f), // 2
		glm::vec3(+0.0f, +0.0f, +1.0f), // Color
		
		glm::vec3(+0.0f, +1.0f, +0.0f), //Normal
		glm::vec3(-1.0f, +1.0f, -1.0f), // 3
		glm::vec3(+1.0f, +1.0f, +1.0f), // Color
		
		glm::vec3(+0.0f, +1.0f, +0.0f), //Normal

		glm::vec3(-1.0f, +1.0f, -1.0f), // 4
		glm::vec3(+1.0f, +0.0f, +1.0f), // Color
		
		glm::vec3(+0.0f, +0.0f, -1.0f), //Normal
		glm::vec3(+1.0f, +1.0f, -1.0f), // 5
		glm::vec3(+0.0f, +0.5f, +0.2f), // Color
		
		glm::vec3(+0.0f, +0.0f, -1.0f), //Normal
		glm::vec3(+1.0f, -1.0f, -1.0f), // 6
		glm::vec3(+0.8f, +0.6f, +0.4f), // Color
		
		glm::vec3(+0.0f, +0.0f, -1.0f), //Normal
		glm::vec3(-1.0f, -1.0f, -1.0f), // 7
		glm::vec3(+0.3f, +1.0f, +0.5f), // Color
		
		glm::vec3(+0.0f, +0.0f, -1.0f), //Normal

		glm::vec3(+1.0f, +1.0f, -1.0f), // 8
		glm::vec3(+0.2f, +0.5f, +0.2f), // Color
		
		glm::vec3(+1.0f, +0.0f, +0.0f), //Normal
		glm::vec3(+1.0f, +1.0f, +1.0f), // 9
		glm::vec3(+0.9f, +0.3f, +0.7f), // Color
		
		glm::vec3(+1.0f, +0.0f, +0.0f), //Normal
		glm::vec3(+1.0f, -1.0f, +1.0f), // 10
		glm::vec3(+0.3f, +0.7f, +0.5f), // Color
		
		glm::vec3(+1.0f, +0.0f, +0.0f), //Normal
		glm::vec3(+1.0f, -1.0f, -1.0f), // 11
		glm::vec3(+0.5f, +0.7f, +0.5f), // Color
		
		glm::vec3(+1.0f, +0.0f, +0.0f), //Normal

		glm::vec3(-1.0f, +1.0f, +1.0f), // 12
		glm::vec3(+0.7f, +0.8f, +0.2f), // Color
		
		glm::vec3(-1.0f, +0.0f, +0.0f), //Normal
		glm::vec3(-1.0f, +1.0f, -1.0f), // 13
		glm::vec3(+0.5f, +0.7f, +0.3f), // Color
		
		glm::vec3(-1.0f, +0.0f, +0.0f), //Normal
		glm::vec3(-1.0f, -1.0f, -1.0f), // 14
		glm::vec3(+0.4f, +0.7f, +0.7f), // Color
		
		glm::vec3(-1.0f, +0.0f, +0.0f), //Normal
		glm::vec3(-1.0f, -1.0f, +1.0f), // 15
		glm::vec3(+0.2f, +0.5f, +1.0f), // Color
		
		glm::vec3(-1.0f, +0.0f, +0.0f), //Normal

		glm::vec3(+1.0f, +1.0f, +1.0f), // 16
		glm::vec3(+0.6f, +1.0f, +0.7f), // Color
		
		glm::vec3(+0.0f, +0.0f, +1.0f), //Normal
		glm::vec3(-1.0f, +1.0f, +1.0f), // 17
		glm::vec3(+0.6f, +0.4f, +0.8f), // Color
		
		glm::vec3(+0.0f, +0.0f, +1.0f), //Normal
		glm::vec3(-1.0f, -1.0f, +1.0f), // 18
		glm::vec3(+0.2f, +0.8f, +0.7f), // Color
		
		glm::vec3(+0.0f, +0.0f, +1.0f), //Normal
		glm::vec3(+1.0f, -1.0f, +1.0f), // 19
		glm::vec3(+0.2f, +0.7f, +1.0f), // Color
		
		glm::vec3(+0.0f, +0.0f, +1.0f), //Normal

		glm::vec3(+1.0f, -1.0f, -1.0f), // 20
		glm::vec3(+0.8f, +0.3f, +0.7f), // Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		glm::vec3(-1.0f, -1.0f, -1.0f), // 21
		glm::vec3(+0.8f, +0.9f, +0.5f), // Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		glm::vec3(-1.0f, -1.0f, +1.0f), // 22
		glm::vec3(+0.5f, +0.8f, +0.5f), // Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		glm::vec3(+1.0f, -1.0f, +1.0f), // 23
		glm::vec3(+0.9f, +1.0f, +0.2f), // Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right 
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_TRIANGLES;

	return ret;

}

Geometry* ShapeGenerator::makeCube2()
{
	if (numGeo < MAX_GEOMETRIES)
	{
		++numGeo;
		Vertex verts[] =
		{
			glm::vec3(-1.0f, +1.0f, +1.0f), // 0
			glm::vec3(+1.0f, +0.0f, +0.0f), // Color
			
			glm::vec3(+0.0f, +1.0f, +0.0f), //Normal
			glm::vec3(+1.0f, +1.0f, +1.0f), // 1
			glm::vec3(+0.0f, +1.0f, +0.0f), // Color
			
			glm::vec3(+0.0f, +1.0f, +0.0f), //Normal
			glm::vec3(+1.0f, +1.0f, -1.0f), // 2
			glm::vec3(+0.0f, +0.0f, +1.0f), // Color
			
			glm::vec3(+0.0f, +1.0f, +0.0f), //Normal
			glm::vec3(-1.0f, +1.0f, -1.0f), // 3
			glm::vec3(+1.0f, +1.0f, +1.0f), // Color
			
			glm::vec3(+0.0f, +1.0f, +0.0f), //Normal

			glm::vec3(-1.0f, +1.0f, -1.0f), // 4
			glm::vec3(+1.0f, +0.0f, +1.0f), // Color
			
			glm::vec3(+0.0f, +0.0f, -1.0f), //Normal
			glm::vec3(+1.0f, +1.0f, -1.0f), // 5
			glm::vec3(+0.0f, +0.5f, +0.2f), // Color
			
			glm::vec3(+0.0f, +0.0f, -1.0f), //Normal
			glm::vec3(+1.0f, -1.0f, -1.0f), // 6
			glm::vec3(+0.8f, +0.6f, +0.4f), // Color
			
			glm::vec3(+0.0f, +0.0f, -1.0f), //Normal
			glm::vec3(-1.0f, -1.0f, -1.0f), // 7
			glm::vec3(+0.3f, +1.0f, +0.5f), // Color
			
			glm::vec3(+0.0f, +0.0f, -1.0f), //Normal

			glm::vec3(+1.0f, +1.0f, -1.0f), // 8
			glm::vec3(+0.2f, +0.5f, +0.2f), // Color
			
			glm::vec3(+1.0f, +0.0f, +0.0f), //Normal
			glm::vec3(+1.0f, +1.0f, +1.0f), // 9
			glm::vec3(+0.9f, +0.3f, +0.7f), // Color
			
			glm::vec3(+1.0f, +0.0f, +0.0f), //Normal
			glm::vec3(+1.0f, -1.0f, +1.0f), // 10
			glm::vec3(+0.3f, +0.7f, +0.5f), // Color
			
			glm::vec3(+1.0f, +0.0f, +0.0f), //Normal
			glm::vec3(+1.0f, -1.0f, -1.0f), // 11
			glm::vec3(+0.5f, +0.7f, +0.5f), // Color
			
			glm::vec3(+1.0f, +0.0f, +0.0f), //Normal

			glm::vec3(-1.0f, +1.0f, +1.0f), // 12
			glm::vec3(+0.7f, +0.8f, +0.2f), // Color
			
			glm::vec3(-1.0f, +0.0f, +0.0f), //Normal
			glm::vec3(-1.0f, +1.0f, -1.0f), // 13
			glm::vec3(+0.5f, +0.7f, +0.3f), // Color
			
			glm::vec3(-1.0f, +0.0f, +0.0f), //Normal
			glm::vec3(-1.0f, -1.0f, -1.0f), // 14
			glm::vec3(+0.4f, +0.7f, +0.7f), // Color
			
			glm::vec3(-1.0f, +0.0f, +0.0f), //Normal
			glm::vec3(-1.0f, -1.0f, +1.0f), // 15
			glm::vec3(+0.2f, +0.5f, +1.0f), // Color
			
			glm::vec3(-1.0f, +0.0f, +0.0f), //Normal

			glm::vec3(+1.0f, +1.0f, +1.0f), // 16
			glm::vec3(+0.6f, +1.0f, +0.7f), // Color
			
			glm::vec3(+0.0f, +0.0f, +1.0f), //Normal
			glm::vec3(-1.0f, +1.0f, +1.0f), // 17
			glm::vec3(+0.6f, +0.4f, +0.8f), // Color
			
			glm::vec3(+0.0f, +0.0f, +1.0f), //Normal
			glm::vec3(-1.0f, -1.0f, +1.0f), // 18
			glm::vec3(+0.2f, +0.8f, +0.7f), // Color
			
			glm::vec3(+0.0f, +0.0f, +1.0f), //Normal
			glm::vec3(+1.0f, -1.0f, +1.0f), // 19
			glm::vec3(+0.2f, +0.7f, +1.0f), // Color
			
			glm::vec3(+0.0f, +0.0f, +1.0f), //Normal

			glm::vec3(+1.0f, -1.0f, -1.0f), // 20
			glm::vec3(+0.8f, +0.3f, +0.7f), // Color
			
			glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
			glm::vec3(-1.0f, -1.0f, -1.0f), // 21
			glm::vec3(+0.8f, +0.9f, +0.5f), // Color
			
			glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
			glm::vec3(-1.0f, -1.0f, +1.0f), // 22
			glm::vec3(+0.5f, +0.8f, +0.5f), // Color
			
			glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
			glm::vec3(+1.0f, -1.0f, +1.0f), // 23
			glm::vec3(+0.9f, +1.0f, +0.2f), // Color
			
			glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		};
		geo[numGeo].m_vertexCount = NUM_ARRAY_ELEMENTS(verts);
		//geo.m_vertexSize = geo.m_vertexCount * sizeof(Vertex);
		geo[numGeo].m_vertexSize = sizeof(verts);
		//ret.vertices = new Vertex[ret.numVertices];
		geo[numGeo].m_vertexStride = sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2, 0, 2, 3, // Top
			4, 5, 6, 4, 6, 7, // Front
			8, 9, 10, 8, 10, 11, // Right 
			12, 13, 14, 12, 14, 15, // Left
			16, 17, 18, 16, 18, 19, // Back
			20, 22, 21, 20, 23, 22, // Bottom
		};
		geo[numGeo].m_indexCount = NUM_ARRAY_ELEMENTS(indices);
		//geo.m_indexSize = geo.m_indexCount * sizeof(GLuint);
		geo[numGeo].m_indexSize = sizeof(indices);
		//ret.indices = new GLuint[ret.numIndices];
		geo[numGeo].m_indexStride = sizeof(GLuint);

		geo[numGeo].vertexFormat = 11;

		RenderEngine::AddGeometry(verts, indices, geo[numGeo]);

		return &geo[numGeo];
	}
	else
	{
		return &fakeGeo;
	}
	
}

ShapeData ShapeGenerator::makeCube(glm::vec3 color)
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f), // 0
		color, // Color
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, +1.0f), // 1
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, -1.0f), // 2
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, +1.0f, -1.0f), // 3
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, +1.0f, -1.0f), // 4
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, -1.0f), // 5
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, -1.0f), // 6
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), // 7
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(+1.0f, +1.0f, -1.0f), // 8
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, +1.0f), // 9
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, +1.0f), // 10
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, -1.0f), // 11
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, +1.0f, +1.0f), // 12
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, +1.0f, -1.0f), // 13
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), // 14
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, +1.0f), // 15
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(+1.0f, +1.0f, +1.0f), // 16
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, +1.0f, +1.0f), // 17
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, +1.0f), // 18
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, +1.0f), // 19
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(+1.0f, -1.0f, -1.0f), // 20
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), // 21
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, +1.0f), // 22
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, +1.0f), // 23
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right 
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_TRIANGLES;

	return ret;
}

ShapeData ShapeGenerator::makeDebugCube(glm::vec3 color)
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f), // 0
		color, // Color
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, +1.0f), // 1
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, -1.0f), // 2
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, +1.0f, -1.0f), // 3
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, +1.0f, -1.0f), // 4
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, -1.0f), // 5
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, -1.0f), // 6
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), // 7
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(+1.0f, +1.0f, -1.0f), // 8
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, +1.0f, +1.0f), // 9
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, +1.0f), // 10
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, -1.0f), // 11
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, +1.0f, +1.0f), // 12
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, +1.0f, -1.0f), // 13
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), // 14
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, +1.0f), // 15
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(+1.0f, +1.0f, +1.0f), // 16
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, +1.0f, +1.0f), // 17
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, +1.0f), // 18
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, +1.0f), // 19
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(+1.0f, -1.0f, -1.0f), // 20
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), // 21
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, +1.0f), // 22
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(+1.0f, -1.0f, +1.0f), // 23
		color,
		
		glm::vec3(0.0f, 0.0f, 0.0f),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLuint indices[] = {
		//0, 1, 2, 0, 2, 3, // Top
		//4, 5, 6, 4, 6, 7, // Front
		//8, 9, 10, 8, 10, 11, // Right 
		//12, 13, 14, 12, 14, 15, // Left
		//16, 17, 18, 16, 18, 19, // Back
		//20, 22, 21, 20, 23, 22, // Bottom
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		8, 9, 9, 10, 10, 11, 11, 8,
		12, 13, 13, 14, 14, 15, 15, 12,
		16, 17, 17, 18, 18, 19, 19, 16,
		20, 21, 21, 22, 22, 23, 23, 20,
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_LINES;

	return ret;
}

ShapeData ShapeGenerator::makeLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color)
{
	ShapeData ret;
	
	Vertex verts[] =
	{
		point1,
		color,
		
		point2,
		color,
		
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLuint indices[] =
	{
		0, 1,
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_LINES;

	return ret;
}

ShapeData ShapeGenerator::makeYGrid(glm::vec3 color, int range)
{
	int numLines = 2 * range + 1;
	int totalLines = 2 * numLines;
	int numVerts = 2 * totalLines;

	ShapeData ret;

	//std::cout << "Draw Grid" << std::endl;
	Vertex* verts = new Vertex[numVerts];
	//std::cout << "Draw Grid" << std::endl;

	for (int j = 0; j < numLines; j++)
	{
		verts[j * 2].position = glm::vec3(j - range, 0, -range);
		//printf("(%f, %f)\n", verts[j * 2].position.x, verts[j * 2].position.z);
		verts[j * 2 + 1].position = glm::vec3(j - range, 0, range);
		//printf("(%f, %f)\n", verts[j * 2 + 1].position.x, verts[j * 2 + 1].position.z);
		//printf("%d\n", j * 2);
		//printf("%d\n", j * 2 + 1);
	}
	for (int j = 0; j < numLines; j++)
	{
		verts[j * 2 + totalLines].position = glm::vec3(-range, 0, j - range);
		//printf("(%f, %f)\n", verts[j * 2 + numLines].position.x, verts[j * 2 + numLines].position.z);
		verts[j * 2 + totalLines + 1].position = glm::vec3(range, 0, j - range);
		//printf("(%f, %f)\n", verts[j * 2 + numLines + 1].position.x, verts[j * 2 + numLines + 1].position.z);
		//printf("%d\n", j * 2 + totalLines);
		//printf("%d\n", j * 2 + totalLines + 1);
	}
	for (int j = 0; j < numVerts; j++)
	{
		verts[j].color = color;
	}

	ret.numVertices = numVerts;
	//ret.vertices = new Vertex[ret.numVertices];
	ret.vertices = verts;
	//memcpy(ret.vertices, verts, sizeof(verts));

	//std::cout << "Draw Grid" << std::endl;
	GLuint* indices = new GLuint[numVerts];
	//std::cout << "Draw Grid" << std::endl;

	for (int j = 0; j < numVerts; j++)
	{
		indices[j] = (GLuint) j;
	}

	ret.numIndices = numVerts;
	//ret.indices = new GLuint[ret.numIndices];
	ret.indices = indices;
	//memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_LINES;
	//std::cout << "Draw Grid" << std::endl;
	return ret;
}

ShapeData ShapeGenerator::makePlane(glm::vec3 upperLeft, glm::vec3 lowerRight)
{
	ShapeData ret;

	//float hypo = glm::distance(upperLeft, lowerRight);

	//float side = hypo * (sqrt(2) / 2);
	
	//glm::vec3 upperRight = glm::vec3(upperLeft.x + lowerRight.x, upperLeft.y - lowerRight.y, upperLeft.z - lowerRight.z);

	Vertex verts[] =
	{
		upperLeft,
		glm::vec3(1.0f, 0.0f, 0.0f),
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(lowerRight.x, 0, upperLeft.z),
		glm::vec3(0.0f, 1.0f, 0.0f),
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		lowerRight,
		glm::vec3(0.0f, 0.0f, 1.0f),
		
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(upperLeft.x, 0, lowerRight.z),
		glm::vec3(1.0f, 1.0f, 0.0f),
		
		glm::vec3(0.0f, 0.0f, 0.0f),
	};

	for (int i = 0; i < 4; i++)
	{
		verts[i].normal = glm::vec3(0, 1, 0);
	}

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLuint indices[] =
	{
		0, 1, 2,
		0, 3, 2,
	};

	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_TRIANGLES;

	return ret;
}

ShapeData ShapeGenerator::makePlane(GLuint dimensions)
{
	ShapeData ret = makePlaneVerts(dimensions);
	ShapeData ret2 = makePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	ret.glEnums = GL_TRIANGLES;
	return ret;
}

ShapeData ShapeGenerator::makePlaneVerts(GLuint dimensions)
{
	ShapeData ret;
	ret.numVertices = dimensions * dimensions;
	float half = dimensions / 2.0f;
	ret.vertices = new Vertex[ret.numVertices];
	for (unsigned int i = 0; i < dimensions; i++)
	{
		for (unsigned int j = 0; j < dimensions; j++)
		{
			Vertex& thisVert = ret.vertices[i * dimensions + j];
			thisVert.position.x = j - half;
			thisVert.position.z = i - half;
			thisVert.position.y = 0;
			thisVert.color = randomColor();
			thisVert.normal = glm::vec3(0, 1, 0);
		}
	}
	return ret;
}

ShapeData ShapeGenerator::makePlaneIndices(GLuint dimensions)
{
	ShapeData ret;
	ret.numIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; //2 triangles per square, 3 indices per triangle
	ret.indices = new GLuint[ret.numIndices];
	int runner = 0;
	for (GLuint row = 0; row < dimensions - 1; row++)
	{
		for (GLuint col = 0; col < dimensions - 1; col++)
		{
			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;

			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;
			ret.indices[runner++] = dimensions * row + col + 1;
		}
	}
	//assert(runner = ret.numIndices);
	return ret;
}

ShapeData ShapeGenerator::makePyramid()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(0.000000, -1.0f, -2.0f), //0
		glm::vec3(+1.0f, +0.0f, +0.0f), //Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		glm::vec3(-2.0f, -1.0f, -0.000000), //1
		glm::vec3(+0.0f, +1.0f, +0.0f), //Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		glm::vec3(-0.000000, -1.0f, 2.0f), //2
		glm::vec3(+0.0f, +0.0f, +1.0f), //Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal
		glm::vec3(2.0f, -1.0f, 0.000000), //3
		glm::vec3(+1.0f, +1.0f, +1.0f), //Color
		
		glm::vec3(+0.0f, -1.0f, +0.0f), //Normal

		glm::vec3(0.000000, 1.0f, 0.000000), //4
		glm::vec3(+1.0f, +0.0f, +1.0f), //Color
		
		glm::normalize(glm::vec3(-1.0f, +1.0f, -1.0f)), //Normal
		glm::vec3(0.000000, -1.0f, -2.0f), //5
		glm::vec3(+0.0f, +0.5f, +0.2f), //Color
		
		glm::normalize(glm::vec3(-1.0f, +1.0f, -1.0f)), //Normal
		glm::vec3(-2.0f, -1.0f, -0.000000), //6
		glm::vec3(+0.8f, +0.6f, +0.4f), //Color
		
		glm::normalize(glm::vec3(-1.0f, +1.0f, -1.0f)), //Normal

		glm::vec3(0.000000, 1.0f, 0.000000), //7
		glm::vec3(+0.3f, +1.0f, +0.5f), //Color
		
		glm::normalize(glm::vec3(+1.0f, +1.0f, -1.0f)), //Normal
		glm::vec3(0.000000, -1.0f, -2.0f), //8
		glm::vec3(+0.2f, +0.5f, +0.2f), //Color
		
		glm::normalize(glm::vec3(+1.0f, +1.0f, -1.0f)), //Normal
		glm::vec3(2.0f, -1.0f, 0.000000), //9
		glm::vec3(+0.9f, +0.3f, +0.7f), //Color
		
		glm::normalize(glm::vec3(+1.0f, +1.0f, -1.0f)), //Normal

		glm::vec3(-2.0f, -1.0f, -0.000000), //10
		glm::vec3(+0.5f, +0.7f, +0.5f), //Color
		glm::normalize(glm::vec3(-1.0f, +1.0f, +1.0f)), //Normal
		glm::vec3(-0.000000, -1.0f, 2.0f), //11
		glm::vec3(+0.7f, +0.8f, +0.2f), //Color
		glm::normalize(glm::vec3(-1.0f, +1.0f, +1.0f)), //Normal
		glm::vec3(0.000000, 1.0f, 0.000000), //12
		glm::vec3(+0.3f, +0.7f, +0.5f), //Color
		glm::normalize(glm::vec3(-1.0f, +1.0f, +1.0f)), //Normal

		glm::vec3(-0.000000, -1.0f, 2.0f), //13
		glm::vec3(+0.5f, +0.7f, +0.3f), //Color
		glm::normalize(glm::vec3(+1.0f, +1.0f, +1.0f)), //Normal
		glm::vec3(2.0f, -1.0f, 0.000000), //14
		glm::vec3(+0.4f, +0.7f, +0.7f), //Color
		glm::normalize(glm::vec3(+1.0f, +1.0f, +1.0f)), //Normal
		glm::vec3(0.000000, 1.0f, 0.000000), //15
		glm::vec3(+0.2f, +0.5f, +1.0f), //Color
		glm::normalize(glm::vec3(+1.0f, +1.0f, +1.0f)), //Normal
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLuint indices[] = {
		2, 1, 0, 3, 2, 0,
		5, 6, 4,
		7, 9, 8,
		10, 11, 12,
		13, 14, 15,
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	ret.glEnums = GL_TRIANGLES;

	return ret;
}

Geometry* ShapeGenerator::makePyramid2()
{
	if (numGeo < MAX_GEOMETRIES)
	{
		Reader read;

		++numGeo;

		Scene* pyramid = read.readSceneFile("..\\..\\Static_Data\\Scenes\\Pyramid.scene");
		geo[numGeo].vertexFormat = pyramid->vertexFormat;
		geo[numGeo].m_vertexCount = pyramid->numVertices;
		//geo.m_vertexSize = geo.m_vertexCount * sizeof(Vertex);
		geo[numGeo].m_vertexSize = pyramid->sizeVertex * pyramid->numVertices;
		//ret.vertices = new Vertex[ret.numVertices];
		geo[numGeo].m_vertexStride = pyramid->sizeVertex;

		geo[numGeo].m_indexCount = pyramid->numIndices;
		//geo.m_indexSize = geo.m_indexCount * sizeof(GLuint);
		geo[numGeo].m_indexSize = pyramid->sizeIndex * pyramid->numIndices;
		//ret.indices = new GLuint[ret.numIndices];
		geo[numGeo].m_indexStride = sizeof(GLuint);

		//printf("%i %i\n", geo[numGeo].m_vertexSize, geo[numGeo].m_indexSize);
		RenderEngine::AddGeometry(pyramid->vertices, pyramid->indices, geo[numGeo]);

		return &geo[numGeo];
	}
	else
	{
		return &fakeGeo;
	}

}

ShapeData ShapeGenerator::generateNormals(const ShapeData& data)
{
	ShapeData ret;
	ret.numVertices = data.numVertices * 2;
	ret.vertices = new Vertex[ret.numVertices];
	glm::vec3 white(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < data.numVertices; i++)
	{
		unsigned int vertIndex = i * 2;
		Vertex& v1 = ret.vertices[vertIndex];
		Vertex& v2 = ret.vertices[vertIndex + 1];
		const Vertex& sourceVertex = data.vertices[i];
		v1.position = sourceVertex.position;
		v2.position = sourceVertex.position + sourceVertex.normal;
		v1.color = v2.color = white;
	}

	ret.numIndices = ret.numVertices;
	ret.indices = new GLuint[ret.numIndices];
	for (GLuint i = 0; i < ret.numIndices; i++)
	{
		ret.indices[i] = i;
	}

	ret.glEnums = GL_LINES;

	return ret;
}

ShapeData ShapeGenerator::makeSphere(glm::vec3 color)
{
	buildSphere(color);
	ShapeData result;
	result.vertices = sphereVerts;
	result.indices = sphereIndices; // no indices used at this time
	result.numVertices = NUMSPHEREVERTS;
	result.numIndices = NUMSPHEREINDICES;
	result.glEnums = GL_LINES;  // Make sure you use GL_LINES

	return result;
}

void ShapeGenerator::buildSphere(glm::vec3 color)
{
	generateSphere(sphereFacets, NUMSPHEREITERATIONS);
	for (GLuint j = 0; j < NUMSPHEREFACETS; ++j)
	{
		sphereVerts[3 * j + 0].position = sphereFacets[j].p1;
		sphereVerts[3 * j + 0].color = color;

		sphereVerts[3 * j + 1].position = sphereFacets[j].p2;
		sphereVerts[3 * j + 1].color = color;

		sphereVerts[3 * j + 2].position = sphereFacets[j].p3;
		sphereVerts[3 * j + 2].color = color;

		sphereIndices[6 * j + 0] = 3 * j + 0;
		sphereIndices[6 * j + 1] = 3 * j + 1;
		sphereIndices[6 * j + 2] = 3 * j + 1;
		sphereIndices[6 * j + 3] = 3 * j + 2;
		sphereIndices[6 * j + 4] = 3 * j + 2;
		sphereIndices[6 * j + 5] = 3 * j + 0;
	}
}

void NormalizeXYZ(glm::vec3* p)
{
	float length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
	p->x /= length;
	p->y /= length;
	p->z /= length;
}

/* Create a triangular facet approximation to a sphere
Return the number of facets created.
The number of facets will be (4^iterations) * 8
*/
int ShapeGenerator::generateSphere(Facet3* facets, int iterations)
{
	glm::vec3 p[6] =
	{
		glm::vec3(+0, +0, +1),
		glm::vec3(+0, +0, -1),
		glm::vec3(-1, -1, +0),
		glm::vec3(+1, -1, +0),
		glm::vec3(+1, +1, +0),
		glm::vec3(-1, +1, +0)
	};
	glm::vec3 pa, pb, pc;
	int numFacets = 0;
	int ntold;

	/* Create the level 0 object */
	float a = (float)(1 / sqrt(2.0));
	for (int i = 0; i < 6; i++)
	{
		p[i].x *= a;
		p[i].y *= a;
	}

	facets[0].p1 = p[0];
	facets[0].p2 = p[3];
	facets[0].p3 = p[4];
	facets[1].p1 = p[0];
	facets[1].p2 = p[4];
	facets[1].p3 = p[5];
	facets[2].p1 = p[0];
	facets[2].p2 = p[5];
	facets[2].p3 = p[2];
	facets[3].p1 = p[0];
	facets[3].p2 = p[2];
	facets[3].p3 = p[3];
	facets[4].p1 = p[1];
	facets[4].p2 = p[4];
	facets[4].p3 = p[3];
	facets[5].p1 = p[1];
	facets[5].p2 = p[5];
	facets[5].p3 = p[4];
	facets[6].p1 = p[1];
	facets[6].p2 = p[2];
	facets[6].p3 = p[5];
	facets[7].p1 = p[1];
	facets[7].p2 = p[3];
	facets[7].p3 = p[2];
	numFacets = 8;
	if (iterations < 1) return numFacets;

	/* Bisect each edge and move to the surface of a unit sphere */
	for (int it = 0; it < iterations; it++)
	{
		ntold = numFacets;
		for (int i = 0; i < ntold; i++)
		{
			pa.x = (facets[i].p1.x + facets[i].p2.x) / 2;
			pa.y = (facets[i].p1.y + facets[i].p2.y) / 2;
			pa.z = (facets[i].p1.z + facets[i].p2.z) / 2;
			pb.x = (facets[i].p2.x + facets[i].p3.x) / 2;
			pb.y = (facets[i].p2.y + facets[i].p3.y) / 2;
			pb.z = (facets[i].p2.z + facets[i].p3.z) / 2;
			pc.x = (facets[i].p3.x + facets[i].p1.x) / 2;
			pc.y = (facets[i].p3.y + facets[i].p1.y) / 2;
			pc.z = (facets[i].p3.z + facets[i].p1.z) / 2;

			NormalizeXYZ(&pa);
			NormalizeXYZ(&pb);
			NormalizeXYZ(&pc);

			facets[numFacets].p1 = facets[i].p1;
			facets[numFacets].p2 = pa;
			facets[numFacets].p3 = pc;

			numFacets++;
			facets[numFacets].p1 = pa;
			facets[numFacets].p2 = facets[i].p2;
			facets[numFacets].p3 = pb;

			numFacets++;
			facets[numFacets].p1 = pb;
			facets[numFacets].p2 = facets[i].p3;
			facets[numFacets].p3 = pc;

			numFacets++;
			facets[i].p1 = pa;
			facets[i].p2 = pb;
			facets[i].p3 = pc;
		}
	}

	return numFacets;
}
