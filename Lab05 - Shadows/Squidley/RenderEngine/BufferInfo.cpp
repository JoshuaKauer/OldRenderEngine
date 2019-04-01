#include "BufferInfo.h"

#define BUFFER_BYTE_SIZE 1024 * 1024 * 12

BufferInfo::BufferInfo()
{
	m_vert_currentByteOffset = 0;
	m_vert_bufferSizeInBytes = 0;
	m_vert_vertexSize = 0;
	m_vert_ID = 9999;
	m_index_currentByteOffset = 0;
	m_index_bufferSizeInBytes = 0;
	m_index_indexSize = 0;
	m_index_ID = 9999;
}

bool BufferInfo::HasRoomForVertices(unsigned totalBytes)
{
	return (m_vert_currentByteOffset + totalBytes < BUFFER_BYTE_SIZE);
}
bool BufferInfo::HasRoomForIndices(unsigned totalBytes)
{
	return (m_index_currentByteOffset + totalBytes < BUFFER_BYTE_SIZE);
}
