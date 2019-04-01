#include "BufferManager.h"
#include "gl/glew.h"
#include "../VertexFormats.h"

#define NUM_BUFFER 50
#define BUFFER_BYTE_SIZE 1024 * 1024 * 12

BufferInfo BufferManager::m_bufferPool[NUM_BUFFER];

int BufferManager::m_numVertexBuffers = 0;
int BufferManager::m_numIndexBuffers = 0;

void BufferManager::Initialize()
{
	
}

void BufferManager::Shutdown()
{
	
}

void BufferManager::AddGeometry(void* vertices, Geometry& mesh)
{
	vertices;
	mesh;
}

void BufferManager::AddGeometry(void* vertices, void* indices, Geometry& mesh)
{
	//const GLsizei vertexStride = mesh.m_vertexStride;
	//char* currentOffset = 0;
	unsigned int attribIndex = 0;
	unsigned int formatOffset = 0;
	for (int j = 0; j < NUM_BUFFER; ++j)
	{
		if (m_bufferPool[j].m_vert_ID == 9999)
		{
			//genbuffer
			m_bufferPool[j].vertFormat = mesh.vertexFormat;
			glGenVertexArrays(1, &m_bufferPool[j].m_VAOID);
			glBindVertexArray(m_bufferPool[j].m_VAOID);

			glGenBuffers(1, &m_bufferPool[j].m_vert_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferPool[j].m_vert_ID);
			glBufferData(GL_ARRAY_BUFFER, BUFFER_BYTE_SIZE, 0, GL_STATIC_DRAW);
			++m_numVertexBuffers;

			glGenBuffers(1, &m_bufferPool[j].m_index_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferPool[j].m_index_ID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, BUFFER_BYTE_SIZE, 0, GL_STATIC_DRAW);
			++m_numIndexBuffers;

			if (mesh.vertexFormat & HasPosition) //SetAttrib(0, positionQuantity, vertexStride, currentOffset, formatOffset);
			{
				glEnableVertexAttribArray(attribIndex);
				glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, mesh.m_vertexStride, (void*)formatOffset);
				formatOffset += sizeof(float) * 3;
				++attribIndex;
			}
			if (mesh.vertexFormat & HasColor)    //SetAttrib(1, colorQuantity, vertexStride, currentOffset, formatOffset);
			{
				glEnableVertexAttribArray(attribIndex);
				glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, mesh.m_vertexStride, (void*)formatOffset);
				formatOffset += sizeof(float) * 3;
				++attribIndex;
			}
			if (mesh.vertexFormat & HasTexture)  //SetAttrib(2, textureQuantity, vertexStride, currentOffset, formatOffset);
			{
				glEnableVertexAttribArray(attribIndex);
				glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, mesh.m_vertexStride, (void*)formatOffset);
				formatOffset += sizeof(float) * 2;
				++attribIndex;
			}
			if (mesh.vertexFormat & HasNormal)   //SetAttrib(3, normalQuantity, vertexStride, currentOffset, formatOffset);
			{
				glEnableVertexAttribArray(attribIndex);
				glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, mesh.m_vertexStride, (void*)formatOffset);
			}
			glBindVertexArray(0);
		}
		if (m_bufferPool[j].HasRoomForVertices(mesh.m_vertexSize))
		{
			if (m_bufferPool[j].HasRoomForIndices(mesh.m_indexSize))
			{
				if (m_bufferPool[j].vertFormat == mesh.vertexFormat)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_bufferPool[j].m_vert_ID);
					glBufferSubData(GL_ARRAY_BUFFER, m_bufferPool[j].m_vert_currentByteOffset, mesh.m_vertexSize, vertices);
					mesh.m_vertexByteOffset = m_bufferPool[j].m_vert_currentByteOffset;
					m_bufferPool[j].m_vert_currentByteOffset += mesh.m_vertexSize;
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferPool[j].m_index_ID);
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_bufferPool[j].m_index_currentByteOffset, mesh.m_indexSize, indices);
					mesh.m_indexByteOffset = m_bufferPool[j].m_index_currentByteOffset;
					m_bufferPool[j].m_index_currentByteOffset += mesh.m_indexSize;
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					//glBindVertexArray(m_bufferPool[j].m_VAOID);
					//glBindVertexArray(0);
					mesh.m_VBID = m_bufferPool[j].m_vert_ID;
					mesh.m_IBID = m_bufferPool[j].m_index_ID;
					mesh.m_VAOID = m_bufferPool[j].m_VAOID;

					break;
				}
				else continue;
			}
		}

	}
}

void BufferManager::AddRenderInfo(Renderable* info)
{
	for (int j = 0; j < NUM_BUFFER; ++j)
	{
		if(m_bufferPool[j].m_vert_ID == info->getMesh()->m_VBID)
		{
			m_bufferPool[j].m_List.addNodes(info);
		}
	}
}

// NOTE: the attribIndex gets incremented by this method, and the formatOffset is modified also
void BufferManager::SetAttrib(int attribIndex, GLint quantity, GLsizei vertexStride, char* currentOffset, unsigned& formatOffset)
{
	const GLboolean isNormalized = GL_FALSE;
	const GLenum dataType = GL_FLOAT;

	glEnableVertexAttribArray(attribIndex);
	glVertexAttribPointer(attribIndex, quantity, dataType, isNormalized, vertexStride, (GLvoid*)(currentOffset + formatOffset));
	formatOffset += quantity * sizeof(dataType); // move over the data to the next (example: move over the position to get to the color)
}