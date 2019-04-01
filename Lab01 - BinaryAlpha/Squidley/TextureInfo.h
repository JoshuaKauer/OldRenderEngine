#pragma once
#include <gl/glew.h>

class TextureInfo
{
public:
	GLuint textureID;
	void loadBMP_custom(const char* imagepath);
	void LoadTexture(const char* imagepath);
};

