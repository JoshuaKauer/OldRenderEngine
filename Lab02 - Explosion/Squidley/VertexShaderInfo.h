#pragma once
#include "gl/glew.h"
class Renderable;
class VertexShaderInfo
{
	typedef void (*uniLocFuncPtr)(Renderable*);
public:
	GLuint programID;
	uniLocFuncPtr callback;
	VertexShaderInfo();
};
