#pragma once
#include "..\\Geometry.h"
#include "..\\Renderable.h"
#include "BufferInfo.h"
class BufferManager
{
public:
	static const GLint positionQuantity = 3;
	static const GLint colorQuantity = 3;
	static const GLint textureQuantity = 2;
	static const GLint normalQuantity = 3;
	static const GLint tangentQuantity = 3;
	static const GLint bitangentQuantity = 3;
	static BufferInfo m_bufferPool[];
	static int m_numVertexBuffers;
	static int m_numIndexBuffers;
	static void Initialize();
	static void Shutdown();
	static void AddGeometry(void* vertices, Geometry& mesh);
	static void AddGeometry(void* vertices, void* indices, Geometry& mesh);
	static void AddRenderInfo(Renderable* info);
	static void SetAttrib(int attribIndex, GLint quantity, GLsizei vertexStride, char* currentOffset, unsigned& formatOffset);
};

