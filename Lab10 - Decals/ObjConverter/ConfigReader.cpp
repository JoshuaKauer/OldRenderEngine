#include "ConfigReader.h"
#include <fstream>
#include <iostream>
#include "../Squidley/Logger.h"

static KeyValuePairs KVP[MAX_KVP];
static int numIter = 0;

bool ConfigReader::initialize(const char* filename)
{
	std::ifstream myInput(filename);
	if (!myInput.good())
	{
		LOG(Error, "File Failed to load");
		exit(1);
	}
	else
	{
		LOG(Info, "File loaded succesfully");
	}
	std::string info;

	while (getline(myInput, info))
	{
		std::stringstream line(info);
		std::string word;
		line >> word;
		if (word == ""){} //Skips blank lines in the file
		else if (word[0] != '/' && word[1] != '/') //Skips the line if it starts off as a comment
		{
			memcpy(KVP[numIter].key, word.c_str(), sizeof(word));
			line >> word;
			if (word[0] == '"') //Checks second word for quotes
			{
				while (word[word.length() - 1] != '"')
				{
					std::string nextWord;
					line >> nextWord;
					word = word + " " + nextWord;
				}
				word = word.substr(1, word.length() - 2);
				memcpy(KVP[numIter].value, word.c_str(), MAX_KEY_LEN);
				numIter++;
			}
			else
			{
				memcpy(KVP[numIter].value, word.c_str(), MAX_VALUE_LEN);
				numIter++;
			}
		}
	}

	myInput.close();
	//std::cout << KVP << std::endl;
	LOG(Info, "ConfigReader Initialized");
	return true;
}

bool ConfigReader::shutdown()
{
	return true;
}

float ConfigReader::getFloatFromString(const char* s)
{
	std::istringstream buffer(s);
	float value;
	buffer >> value;
	return value;
}

int ConfigReader::getIntFromString(const char* s)
{
	std::istringstream buffer(s);
	int value;
	buffer >> value;
	return value;
}

int ConfigReader::getFloatsFromString(const char* s, float values[], int numValues)
{
	std::istringstream buffer(s);
	std::string word;
	for (int i = 0; i < numValues; ++i)
	{
		buffer >> word;
		values[i] = getFloatFromString(word.c_str());
	}
	return numValues;
}

const char* ConfigReader::findValueForKey(const char* pKey)
{
	for (int i = 0; i < numIter; ++i)
	{
		if (strcmp(KVP[i].key, pKey) == 0)
		{
			return KVP[i].value;
		}
	}
	return 0;
}

const char* ConfigReader::findNeededValueForKey(const char* pKey)
{
	if (findValueForKey(pKey) == 0)
	{
		std::string message = "Cannot find value for " + string(pKey);
		//ASSERT(true, message.c_str());
		LOG(Error, message.c_str());
		std::cout << message << std::endl;
		exit(1);
	}
	else
	{
		return findValueForKey(pKey);
	}
}