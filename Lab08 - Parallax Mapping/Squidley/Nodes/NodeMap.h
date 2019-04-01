#pragma once
#include "Nodes.h"

class NodeMap
{
public:
	NodeMap() : m_nodes(0), m_links(0), m_nodeData(0) {}
	~NodeMap()
	{
		char* p = reinterpret_cast<char*> (m_nodeData);
		delete[] p;
	}
	bool ReadPathingFile(const char* filename);
	void Dump();
public:
	Node* m_nodes;
	NodeLink* m_links;
	PathingNodes* m_nodeData;
};