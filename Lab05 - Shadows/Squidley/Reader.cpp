#include "Reader.h"
#include "Logger.h"

Scene* Reader::readSceneFile(const char* filename)
{
	std::ifstream inputStream(filename, std::ios::binary | std::ios::in);
	if (!inputStream.good())
	{
		printf("ERROR : Unable to open scene %s.\n", filename);
		return 0;
	}
	int dataLen;
	inputStream.read(reinterpret_cast<char*> (&dataLen), sizeof(dataLen));
	//printf("Reading %d bytes.\n", dataLen);

	char* data = new char[dataLen];
	assert(data);
	std::fill(data, data + dataLen - 1, 0);

	inputStream.read(data, dataLen);
	inputStream.close();

	Scene* scene = reinterpret_cast<Scene*> (data);
	char* p = data;
	p += sizeof(Scene);
	scene->vertices = reinterpret_cast<void*> (p);
	p += scene->numVertices * scene->sizeVertex;
	scene->indices = reinterpret_cast<void*> (p);

	return scene;
}

void Reader::displayVec3(glm::vec3* vec)
{
	printf("<%f, %f, %f>\n", vec->x, vec->y, vec->z);
}

void Reader::displayScene(Scene* scene)
{
	printf("NumVerts : %d\n", scene->numVertices);
	printf("NumIndices : %d\n", scene->numIndices);
	printf("sizeVerts : %d\n", scene->sizeVertex);
	printf("sizeIndices : %d\n", scene->sizeIndex);
	for (int j = 0; j < scene->numVertices; ++j)
	{
		//printf("Position[%d] = ", j); displayVec3(&scene->vertices[j].position);
	}
	for (int j = 0; j < scene->numVertices; ++j)
	{
		//printf("Color[%d] = ", j); displayVec3(&scene->vertices[j].color);
	}
	for (int j = 0; j < scene->numVertices; ++j)
	{
		//printf("Normal[%d] = ", j); displayVec3(&scene->vertices[j].normal);
	}
	for (int j = 0; j < scene->numIndices; ++j)
	{
		//GLuint index = scene->indices[j];
		//std::cout << index << "\n";
	}
	glm::vec3* p = reinterpret_cast<glm::vec3*> (scene->vertices);
	for (int j = 0; j < 3 * scene->numVertices; ++j)
	{
		printf("Vec[%d] = ", j); displayVec3(&p[j]);
	}
}

//void Reader::showCurrentDir()
//{
//	char temp[_MAX_PATH];
//	_getcwd(temp, _MAX_PATH);
//	printf("CurrentDir = [%s].\n", temp);
//}