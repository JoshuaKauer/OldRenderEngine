#pragma once
#include "../ECS/Component.h"
#include "../Nodes/NodeMap.h"
#include "../PathFinding.h"

class AStarComponent : public Component
{
	NodeMap* m_Map;
	Node* path;
	Node target;
	PathFinding m_pathFinder;
	int pathCount = 0;
public:
	bool done = false;
	bool setPath(Node start, Node end);
	bool Update(float) override;
	void setNodeMap(NodeMap*);
};
