#include "Reader.h"

std::string Reader::readTextFile(const char* fileName)
{
	std::ifstream myInput(fileName);
	if (!myInput.good())
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(std::istreambuf_iterator<char>(myInput), std::istreambuf_iterator<char>());
}

int Reader::checkLineInfo(char char1, char char2, char char3)
{
	if (char1 == '\n')
	{
		if (char2 == 'v')
		{
			if (char3 == ' ')
			{
				//std::cout << "Vertex Info" << std::endl;
				vertexCount++;
				return 1;
			}
			if (char3 == 't')
			{
				//std::cout << "Texture Info" << std::endl;
				textureCount++;
				return 2;
			}
			if (char3 == 'n')
			{
				//std::cout << "Normal Info" << std::endl;
				normalCount++;
				return 3;
			}
		}
		if (char2 == 'f')
		{
			//std::cout << "Index Info" << std::endl;
			indexCount++;
			return 4;
		}
	}
	return 0;
}

void Reader::getLineInfo(const char* adapter, int length)
{
	int count = 0;
	for (int i = 99; i < length; i++)
	{
		//printf("%c", adapter[i]);
		if (checkLineInfo(adapter[i], adapter[i + 1], adapter[i + 2]) != 0)
		{
			arrayLocation[count] = i + 1;
			count++;
		}
	}
	textureCount += vertexCount;
	normalCount += textureCount;
	indexCount += normalCount;
	adapt = adapter;

	readVertexInfo();
	readTextureInfo();
	readNormalInfo();
	readIndexInfo();
}

void Reader::readLine(int start, int end)
{
	std::string line;
	char* temp = new char[end - start];
	int count = 0;
	for (start; start < end; start++)
	{
		printf("%c", adapt[start]);

		if (adapt[start] == '\n')
		{
			//line.append(temp);
			//printf("%s", line);
			return;
		}
		//line += adapt[start];
		//temp[count++] = adapt[start];
		//printf("%c", temp[count]);
	}
}

void Reader::readLine(int start)
{
	std::string line;
	while (adapt[start] != '\n')
	{
		printf("%c", adapt[start]);
		//line += adapt[start];
		start++;
	}
	//printf("%s", line);
	std::cout << std::endl;
}

void Reader::readVertexInfo()
{
	for (int i = 0; i < vertexCount; i++)
	{
		readLine(arrayLocation[i], arrayLocation[i + 1]);
		//std::cout << i << std::endl;
	}
}

void Reader::readTextureInfo()
{
	for (int i = vertexCount; i < textureCount; i++)
	{
		readLine(arrayLocation[i], arrayLocation[i + 1]);
		//std::cout << i << std::endl;
	}
}

void Reader::readNormalInfo()
{
	for (int i = textureCount; i < normalCount; i++)
	{
		readLine(arrayLocation[i], arrayLocation[i + 1]);
		//std::cout << i << std::endl;
	}
}

void Reader::readIndexInfo()
{
	for (int i = normalCount; i < indexCount; i++)
	{
		readLine(arrayLocation[i]);
		//std::cout << i << std::endl;
	}
}