#pragma once
#include "../List.h"
class BufferInfo
{
public:
	List m_List;
	int vertFormat;
	unsigned m_vert_currentByteOffset;
	unsigned m_vert_bufferSizeInBytes;
	unsigned m_vert_vertexSize;
	unsigned m_vert_ID;
	unsigned m_index_currentByteOffset;
	unsigned m_index_bufferSizeInBytes;
	unsigned m_index_indexSize;
	unsigned m_index_ID;
	unsigned m_VAOID;
	BufferInfo();
	bool HasRoomForVertices(unsigned totalBytes);
	bool HasRoomForIndices(unsigned totalBytes);
};

