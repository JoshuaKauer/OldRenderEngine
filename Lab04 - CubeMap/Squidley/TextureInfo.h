#pragma once
#include <gl/glew.h>

class TextureInfo
{
public:
	GLuint textureID;
	GLuint shadowTextureID;
	bool isCubeMap = false;
	void loadBMP_custom(const char* imagepath);
	void readBMP(unsigned int* width, unsigned int* height, unsigned char* &image, const char* imagepath);
	void loadCubeMap(const GLchar* faces[]);
	void LoadDDS(const char* imagepath);
};

