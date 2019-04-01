#pragma once
#include <sstream>
#include <GL\glew.h>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning(pop)

class ObjParser
{
	int vertCount = 0;
	int normCount = 0;
	int textCount = 0;
	int tanCount = 0;
	int bitanCount = 0;

	void getVerts();
	void getColors();
	void getTextures();
	void getNormals();
	void getTangents();
	void getBiTangents();

public:
	short vertexFormat = 0;
	const char* colorOverride;
	const char* textureOverride;
	const char* normalOverride;

	glm::vec3* finalVerts;
	glm::vec3* finalNormals;
	glm::vec2* finalTextures;
	glm::vec3* finalColors;
	glm::vec3* finalTans;
	glm::vec3* finalBiTans;
	GLuint* finalIndices;

	int indCount = 0;

	void readTextFile(const char* fileName);

	bool parseLine(std::string line);

	bool addVertex(std::stringstream &parser);

	bool addNormal(std::stringstream &parser);

	bool addTexture(std::stringstream &parser);

	bool addIndex(std::stringstream &parser);

	void finalArrays();
};

