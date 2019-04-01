#pragma once
#include <sstream>

#define MAX_KEY_LEN 75
#define MAX_VALUE_LEN 100
#define MAX_KVP 75

struct KeyValuePairs
{
	char key[MAX_KEY_LEN];
	char value[MAX_VALUE_LEN];
};

class ConfigReader
{
public:	
	static bool initialize(const char* filename = "..\\Data\\Config.txt");

	static bool shutdown();

	static float getFloatFromString(const char* s);

	static int getIntFromString(const char* s);

	static int getFloatsFromString(const char* s, float values[], int numValues);

	static const char* findValueForKey(const char* pKey);

	static const char* findNeededValueForKey(const char* pKey);
};
