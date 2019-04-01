#pragma once
#include <GL\glew.h>

class Geometry
{
public:
	int vertexFormat;
	void* vertices;
	void* indices;
	unsigned m_vertexByteOffset;
	unsigned m_vertexCount;
	unsigned m_vertexStride;
	unsigned m_VBID;
	unsigned m_vertexSize;
	unsigned m_indexByteOffset;
	unsigned m_indexCount;
	unsigned m_indexStride;
	unsigned m_IBID;
	unsigned m_indexSize;
	unsigned m_VAOID;
	bool m_use16BitIndices;
	bool IsIndexed();
	bool Uses16BitIndices();
	GLenum glEnums;

};

