#include "BinaryOutput.h"
#include "../Squidley/VertexFormats.h"

void BinaryOutput::writeCustomBinaryFile(const char* filename, short vertForm, int numVerts, int numIndices, glm::vec3* verts, glm::vec3* color, glm::vec2* texture, glm::vec3* normals, GLuint* indices)
{
	int totalBytes = 0;
	std::ofstream outputStream(filename, std::ios::binary | std::ios::out);

	outputStream.seekp(0);
	writeInt(outputStream, totalBytes);

	int vertexSize = 0;

	switch (vertForm)
	{
	case PositionOnly :
		vertexSize = sizeof(glm::vec3);
		break;
	case PositionColor :
		vertexSize = sizeof(glm::vec3) * 2;
		break;
	case PositionTexture :
		vertexSize = sizeof(glm::vec3) + sizeof(glm::vec2);
		break;
	case PositionNormal :
		vertexSize = sizeof(glm::vec3) * 2;
		break;
	case PositionColorNormal :
		vertexSize = sizeof(glm::vec3) * 3;
		break;
	case PositionTextureNormal :
		vertexSize = sizeof(glm::vec3) * 2 + sizeof(glm::vec2);
		break;
	case PositionColorTexture :
		vertexSize = sizeof(glm::vec3) * 2 + sizeof(glm::vec2);
		break;
	case PositionColorTextureNormal :
		vertexSize = sizeof(glm::vec3) * 3 + sizeof(glm::vec2);
		break;
	default :
		printf("No given value for vertexSize\nvertexSize : %i\n", vertexSize);
		exit(1);
		break;
	}
	totalBytes += writeHeader(outputStream, vertForm, numVerts, numIndices, vertexSize, sizeof(GLuint));
	totalBytes += writeVertexData(outputStream, vertForm, numVerts, verts, color, texture, normals);
	totalBytes += writeIndices(outputStream, numIndices, indices);

	outputStream.seekp(0);
	writeInt(outputStream, totalBytes);
	outputStream.close();
	printf("Total : wrote %d bytes.\n", totalBytes);
}

int BinaryOutput::writeHeader(std::ofstream& out, short vertForm, int numVerts, int numIndices, int sizeVerts, int sizeIndices)
{
	int totalBytes = 0;
	totalBytes += writeInt(out, vertForm);
	totalBytes += writeInt(out, numVerts);
	totalBytes += writeInt(out, numIndices);
	totalBytes += writeInt(out, sizeVerts);
	totalBytes += writeInt(out, sizeIndices);
	totalBytes += writePointer(out, 2);
	return totalBytes;
}

int BinaryOutput::writeInt(std::ofstream& out, int value)
{
	int size = sizeof(int);
	out.write(reinterpret_cast<char*> (&value), size);
	return size;
}

int BinaryOutput::writePointer(std::ofstream& out, int count)
{
	int size = sizeof(void*);
	void* pointer = 0;
	for (int j = 0; j < count; ++j)
	{
		out.write(reinterpret_cast<char*> (&pointer), size);
	}
	return count * size;
}

int BinaryOutput::writeVertexData(std::ofstream& out, short vertForm, int numVerts, glm::vec3* verts, glm::vec3* color, glm::vec2* texture, glm::vec3* normals)
{
	int totalBytes = 0;
	/*for (int j = 0; j < numVerts; ++j)
	{
		totalBytes += writeVec3(out, verts[j]);
		totalBytes += writeVec3(out, color[j]);
		totalBytes += writeVec2(out, texture[j]);
		totalBytes += writeVec3(out, normals[j]);
	}*/
	switch (vertForm)
	{
	case PositionOnly:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
		}
		break;
	case PositionColor:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec3(out, color[j]);
		}
		break;
	case PositionTexture:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec2(out, texture[j]);
		}
		break;
	case PositionNormal:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec3(out, normals[j]);
		}
		break;
	case PositionColorNormal:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec3(out, color[j]);
			totalBytes += writeVec3(out, normals[j]);
		}
		break;
	case PositionTextureNormal:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec2(out, texture[j]);
			totalBytes += writeVec3(out, normals[j]);
		}
		break;
	case PositionColorTexture:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec3(out, color[j]);
			totalBytes += writeVec2(out, texture[j]);
		}
		break;
	case PositionColorTextureNormal:
		for (int j = 0; j < numVerts; ++j)
		{
			totalBytes += writeVec3(out, verts[j]);
			totalBytes += writeVec3(out, color[j]);
			totalBytes += writeVec2(out, texture[j]);
			totalBytes += writeVec3(out, normals[j]);
		}
		break;
	default:
		printf("Positions, Colors, Textures, Normals aren't being defined.\nVertex Format : %i\n", vertForm);
		exit(1);
		break;
	}
	return totalBytes;
}

int BinaryOutput::writeVec2(std::ofstream& out, glm::vec2 vec)
{
	int size = sizeof(vec);
	out.write(reinterpret_cast<char*> (&vec), size);
	return size;
}

int BinaryOutput::writeVec3(std::ofstream& out, glm::vec3 vec)
{
	int size = sizeof(vec);
	out.write(reinterpret_cast<char*> (&vec), size);
	return size;
}

int BinaryOutput::writeIndices(std::ofstream& out, int numIndices, GLuint* indices)
{
	int totalBytes = 0;
	for (int j = 0; j < numIndices; ++j)
	{
		totalBytes += writeGLuint(out, indices[j]);
	}
	return totalBytes;
}

int BinaryOutput::writeGLuint(std::ofstream& out, GLuint value)
{
	int size = sizeof(GLuint);
	out.write(reinterpret_cast<char*> (&value), size);
	return size;
}
