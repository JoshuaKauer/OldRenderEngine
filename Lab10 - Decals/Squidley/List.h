#pragma once
#include "ListNode.h"

class List
{
public:
	ListNode* head = 0;
	void addNodes(Renderable*);
	void drawNodes(float* cameraPos = 0, float* cameraViewDir = 0);
	void drawNodes(GLuint programID, uniLocFuncPtr callback);
};

