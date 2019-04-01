#pragma once
#include "gl/glew.h"

class Renderable;

typedef void(*uniLocFuncPtr)(Renderable*);

class VertexShaderInfo
{
public:
	GLuint programID;
	uniLocFuncPtr callback;
	VertexShaderInfo();
};
