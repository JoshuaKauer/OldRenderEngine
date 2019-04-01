#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm/glm.hpp"
#pragma warning(pop)

class Node
{
public: //private?
	Node() : id((unsigned)-1) {}
	glm::vec3 position;
	unsigned id;
};

class NodeLink
{
public:
	NodeLink() : to((unsigned)-1), from((unsigned)-1), cost(99999999.0f) {}
public: //private?
	unsigned to;
	unsigned from;
	float cost;
};

class PathingNodes
{
public:
	unsigned numNodes;
	unsigned numLinks;
	float nodeRadius;
};