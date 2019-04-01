#pragma once
#include "Nodes/Nodes.h"

class AStarPath
{
public:
	float fitness;
	float g;
	Node current;
	AStarPath* parent;
};
