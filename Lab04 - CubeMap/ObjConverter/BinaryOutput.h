#pragma once
#include <fstream>
#include <GL\glew.h>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning(pop)

enum VertexFormats;

class BinaryOutput
{
public:
	void writeCustomBinaryFile(const char* filename, short vertForm, int numVerts, int numIndices, glm::vec3* verts, glm::vec3* color, glm::vec2* texture, glm::vec3* normals, GLuint* indices);
	int writeHeader(std::ofstream& out, short vertForm, int numVerts, int numIndices, int sizeVerts, int sizeIndices);
	int writeInt(std::ofstream& out, int value);
	int writePointer(std::ofstream& out, int count);
	int writeVertexData(std::ofstream& out, short vertForm, int numVerts, glm::vec3* verts, glm::vec3* color, glm::vec2* texture, glm::vec3* normals);
	int writeVec2(std::ofstream& out, glm::vec2 vec);
	int writeVec3(std::ofstream& out, glm::vec3 vec);
	int writeIndices(std::ofstream& out, int numIndices, GLuint* indices);
	int writeGLuint(std::ofstream& out, GLuint value);
};

