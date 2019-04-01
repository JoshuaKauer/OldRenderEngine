#include "ListNode.h"

ListNode::ListNode(Renderable* r)
{
	node = r;
}

void ListNode::addNode(Renderable* r)
{
	if (next == 0)
	{
		next = new ListNode(r);
	}
	else
	{
		next->addNode(r);
	}
}

void ListNode::drawNode()
{
	node->Draw();
	
	if (next != 0)
	{
		next->drawNode();
	}
}
