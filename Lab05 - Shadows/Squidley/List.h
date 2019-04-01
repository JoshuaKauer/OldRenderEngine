#pragma once
#include "ListNode.h"

class List
{
public:
	ListNode* head = 0;
	void addNodes(Renderable*);
	void drawNodes();
	void drawNodes(GLuint programID, uniLocFuncPtr callback);
};

