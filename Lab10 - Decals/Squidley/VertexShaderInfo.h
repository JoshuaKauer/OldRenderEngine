#pragma once
#include "gl/glew.h"

class Renderable;

typedef void(*uniLocFuncPtr)(Renderable*, float* cameraPos, float* cameraViewDir);

class VertexShaderInfo
{
public:
	GLuint programID;
	uniLocFuncPtr callback;
	VertexShaderInfo();
};
