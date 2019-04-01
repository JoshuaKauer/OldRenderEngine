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

void List::drawNodes()
{
	if (head != 0)
	{
		head->drawNode();
	}
}
