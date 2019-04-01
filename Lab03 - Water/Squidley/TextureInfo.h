#pragma once
#include <gl/glew.h>

class TextureInfo
{
public:
	GLuint textureID;
	GLuint shadowTextureID;
	void loadBMP_custom(const char* imagepath);
	void LoadDDS(const char* imagepath);
};

