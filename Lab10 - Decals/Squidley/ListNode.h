#pragma once
#include "Renderable.h"
#include "gl/glew.h"

class ListNode
{
public:
	ListNode* next = 0;
	Renderable* node;
	ListNode(Renderable*);
	void addNode(Renderable*);
	void drawNode(float* cameraPos = 0, float* cameraViewDir = 0);
	void drawNode(GLuint programID, uniLocFuncPtr callback);
};

