#include "ObjParser.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "ConfigReader.h"
#include "../Squidley/VertexFormats.h"
#include <time.h>

ConfigReader config;

bool useTexture = false; 
bool useNormal = false;

glm::vec3 verts[3000000];
glm::vec3 normals[3000000];
glm::vec2 textures[3000000];
glm::vec3 tans[3000000];
glm::vec3 bitans[3000000];
GLuint vertIndices[3000000];
GLuint textIndices[3000000];
GLuint normIndices[3000000];
GLuint tanIndices[3000000];
GLuint bitanIndices[3000000];

void ObjParser::readTextFile(const char* fileName)
{
	std::ifstream myInput(fileName);
	if (!myInput.good())
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	//myInput.getline(myInput, 80);
	std::string line;
	while (std::getline(myInput, line))
	{
		if (!parseLine(line))
		{
			std::cout << "Error Occurred while Loading" << std::endl;
		}
	}
	//return std::string(std::istreambuf_iterator<char>(myInput), std::istreambuf_iterator<char>());
	finalArrays();
	myInput.close();
}

bool ObjParser::parseLine(std::string line)
{
	std::stringstream parser(line);
	std::string word;
	parser >> word;

	//printf("%s\n", parser.str().c_str());
	//printf("%s\n", word.c_str());

	if (parser.fail())//blank line
	{
		
	}
	else if (word[0] == '#')//comment
	{

	}
	else if (word == "v")//vertex
	{
		if (!addVertex(parser))
		{
			return false;
		}
	}
	else if (word == "vn")//normal
	{
		useNormal = true;
		if (!addNormal(parser))
		{
			return false;
		}
	}
	else if (word == "vt")//texture
	{
		useTexture = true;
		if (!addTexture(parser))
		{
			return false;
		}
	}
	else if (word == "f")
	{
		if (!addIndex(parser))
		{
			return false;
		}
	}
	else
	{
		//Print I am ignoring the following content:
	}

	return true;
}

bool ObjParser::addVertex(std::stringstream &parser)
{
	std::string vecX, vecY, vecZ;

	parser >> vecX;
	parser >> vecY;
	parser >> vecZ;

	verts[vertCount].x = config.getFloatFromString(vecX.c_str());
	verts[vertCount].y = config.getFloatFromString(vecY.c_str());
	verts[vertCount].z = config.getFloatFromString(vecZ.c_str());

	/*printf("%s ", vecX.c_str());
	printf("%s ", vecY.c_str());
	printf("%s\n", vecZ.c_str());*/

	vertCount++;

	return true;
}

bool ObjParser::addNormal(std::stringstream &parser)
{
	std::string vecX, vecY, vecZ;

	parser >> vecX;
	parser >> vecY;
	parser >> vecZ;

	normals[normCount].x = config.getFloatFromString(vecX.c_str());
	normals[normCount].y = config.getFloatFromString(vecY.c_str());
	normals[normCount].z = config.getFloatFromString(vecZ.c_str());

	/*printf("%s ", vecX.c_str());
	printf("%s ", vecY.c_str());
	printf("%s\n", vecZ.c_str());*/

	normCount++;

	return true;
}

bool ObjParser::addTexture(std::stringstream &parser)
{
	std::string vecX, vecY;

	parser >> vecX;
	parser >> vecY;

	textures[textCount].x = config.getFloatFromString(vecX.c_str());
	textures[textCount].y = config.getFloatFromString(vecY.c_str());

	/*printf("%s ", vecX.c_str());
	printf("%s ", vecY.c_str());
	printf("%s\n", vecZ.c_str());*/

	textCount++;

	return true;
}

bool ObjParser::addIndex(std::stringstream &parser)
{
	std::string index;
	std::string test = parser.str();
	//parser.str().replace(parser.str().begin(), parser.str().end(), '/', ' ');
	std::replace(test.begin(), test.end(), '/', ' ');
	std::istringstream secondPar(test.c_str());

	secondPar >> index;
	secondPar >> index;
	vertIndices[indCount] = (GLuint)config.getFloatFromString(index.c_str());
	secondPar >> index;
	textIndices[indCount] = (GLuint)config.getFloatFromString(index.c_str());
	secondPar >> index;
	normIndices[indCount++] = (GLuint)config.getFloatFromString(index.c_str());

	secondPar >> index;
	vertIndices[indCount] = (GLuint)config.getFloatFromString(index.c_str());
	secondPar >> index;
	textIndices[indCount] = (GLuint)config.getFloatFromString(index.c_str());
	secondPar >> index;
	normIndices[indCount++] = (GLuint)config.getFloatFromString(index.c_str());
	
	secondPar >> index;
	vertIndices[indCount] = (GLuint)config.getFloatFromString(index.c_str());
	secondPar >> index;
	textIndices[indCount] = (GLuint)config.getFloatFromString(index.c_str());
	secondPar >> index;
	normIndices[indCount++] = (GLuint)config.getFloatFromString(index.c_str());
	
	return true;
}

void ObjParser::finalArrays()
{
	colorOverride = ConfigReader::findValueForKey("OutputColor");
	textureOverride = ConfigReader::findValueForKey("OutputTexture");
	normalOverride = ConfigReader::findValueForKey("OutputNormal");
	finalIndices = new GLuint[indCount];
	for (int i = 0; i < indCount; i++)
	{
		finalIndices[i] = (GLuint)i;
	}
	switch (vertexFormat)
	{
	case PositionOnly :
		getVerts();
		break;
	case PositionColor :
		getVerts();
		getColors();
		break;
	case PositionTexture :
		getVerts();
		getTextures();
		break;
	case PositionNormal :
		getVerts();
		getNormals();
		break;
	case PositionColorNormal :
		getVerts();
		getColors();
		getNormals();
		break;
	case PositionTextureNormal :
		getVerts();
		getTextures();
		getNormals();
		break;
	case PositionColorTexture :
		getVerts();
		getColors();
		getTextures();
		break;
	case PositionColorTextureNormal :
		getVerts();
		getColors();
		getTextures();
		getNormals();
		break;
	case PositionTextureNormalTangentBiTangent :
		getVerts();
		getTextures();
		getNormals();
		getTangents();
		getBiTangents();
		break;
	default :
		printf("Positions, Colors, Textures, Normals aren't being defined.\nVertex Format : %i\n", vertexFormat);
		exit(1);
		break;
	}
}

void ObjParser::getVerts()
{
	finalVerts = new glm::vec3[indCount];
	for (int i = 0; i < indCount; i++)
	{
		finalVerts[i] = verts[vertIndices[i] - 1];
	}
}

//Helper function for following function
glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

void ObjParser::getColors()
{
	srand((unsigned)time(NULL));
	finalColors = new glm::vec3[indCount];
	if (strcmp(colorOverride, "random") == 0)
	{
		for (int i = 0; i < indCount; i++)
		{
			finalColors[i] = randomColor();
		}
	}
	else
	{
		float colors[3];
		ConfigReader::getFloatsFromString(ConfigReader::findValueForKey("OutputColor"), colors, 3);
		for (int i = 0; i < indCount; ++i)
		{
			finalColors[i].x = colors[0];
			finalColors[i].y = colors[1];
			finalColors[i].z = colors[2];
		}
	}
}

void ObjParser::getTextures()
{
	srand((unsigned)time(NULL));
	finalTextures = new glm::vec2[indCount];
	if (useTexture)
	{
		for (int i = 0; i < indCount; ++i)
		{
			finalTextures[i] = textures[textIndices[i] - 1];
		}
	}
	else
	{
		if (strcmp(textureOverride, "random") == 0)
		{
			for (int i = 0; i < indCount; ++i)
			{
				finalTextures[i].x = randomColor().x;
				finalTextures[i].y = randomColor().y;
			}
		}
		else
		{
			float texture[2];
			ConfigReader::getFloatsFromString(ConfigReader::findValueForKey("OutputTexture"), texture, 2);
			for (int i = 0; i < indCount; ++i)
			{
				finalTextures[i].x = texture[0];
				finalTextures[i].y = texture[1];
			}
		}
	}
}

void ObjParser::getNormals()
{
	srand((unsigned)time(NULL));
	finalNormals = new glm::vec3[indCount];
	if (useNormal)
	{
		for (int i = 0; i < indCount; i++)
		{
			finalNormals[i] = normals[normIndices[i] - 1];
		}
	}
	else
	{
		if (strcmp(normalOverride, "random") == 0)
		{
			for (int i = 0; i < indCount; i++)
			{
				finalNormals[i] = randomColor();
			}
		}
		else
		{
			float _normals[3];
			ConfigReader::getFloatsFromString(ConfigReader::findValueForKey("OutputNormal"), _normals, 3);
			for (int i = 0; i < indCount; ++i)
			{
				finalNormals[i].x = _normals[0];
				finalNormals[i].y = _normals[1];
				finalNormals[i].z = _normals[2];
			}
		}
	}
}

void ObjParser::getTangents()
{
	finalTans = new glm::vec3[indCount];
	int currentIndex = 0;
	for (int i = 0; i < indCount; i += 3)
	{
		glm::vec3 edge1 = finalVerts[currentIndex + 1] - finalVerts[currentIndex];
		glm::vec3 edge2 = finalVerts[currentIndex + 2] - finalVerts[currentIndex];
		glm::vec2 deltaUV1 = finalTextures[currentIndex + 1] - finalTextures[currentIndex];
		glm::vec2 deltaUV2 = finalTextures[currentIndex + 2] - finalTextures[currentIndex];

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		finalTans[i].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		finalTans[i].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		finalTans[i].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		finalTans[i] = glm::normalize(finalTans[i]);
		finalTans[i + 1] = finalTans[i];
		finalTans[i + 2] = finalTans[i];
		currentIndex += 3;
	}
}

void ObjParser::getBiTangents()
{
	finalBiTans = new glm::vec3[indCount];
	int currentIndex = 0;
	for (int i = 0; i < indCount; i += 3)
	{
		glm::vec3 edge1 = finalVerts[currentIndex + 1] - finalVerts[currentIndex];
		glm::vec3 edge2 = finalVerts[currentIndex + 2] - finalVerts[currentIndex];
		glm::vec2 deltaUV1 = finalTextures[currentIndex + 1] - finalTextures[currentIndex];
		glm::vec2 deltaUV2 = finalTextures[currentIndex + 2] - finalTextures[currentIndex];

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		finalBiTans[i].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		finalBiTans[i].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		finalBiTans[i].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		finalBiTans[i] = glm::normalize(finalBiTans[i]);
		finalBiTans[i + 1] = finalBiTans[i];
		finalBiTans[i + 2] = finalBiTans[i];
		currentIndex += 3;
	}
}
