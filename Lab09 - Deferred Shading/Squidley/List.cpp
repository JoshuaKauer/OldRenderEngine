#include "List.h"

void List::addNodes(Renderable* r)
{
	if (head == 0)
	{
		head = new ListNode(r);
	}
	else
	{
		head->addNode(r);
	}
}

void List::drawNodes(float* cameraPos, float* cameraViewDir)
{
	if (head != 0)
	{
		head->drawNode(cameraPos, cameraViewDir);
	}
}

void List::drawNodes(GLuint programID, uniLocFuncPtr callback)
{
	if (head != 0)
	{
		head->drawNode(programID, callback);
	}
}
