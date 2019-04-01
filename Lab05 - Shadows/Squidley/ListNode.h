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
	void drawNode();
	void drawNode(GLuint programID, uniLocFuncPtr callback);
};

