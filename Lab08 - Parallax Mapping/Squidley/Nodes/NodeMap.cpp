#include "NodeMap.h"
#include <fstream>

bool NodeMap::ReadPathingFile(const char* filename)
{
	std::ifstream inputStream(filename, std::ios::binary | std::ios::in);

	if (!inputStream.good())
	{
		printf("ERROR : Unable to open file for reading [%s]\n", filename);
		return false;
	}
	//if (!inputStream) printf("not stream\n");
	//if (!inputStream.is_open()) printf("not open\n");

	inputStream.seekg(0, std::ios::end);
	unsigned numBytes = static_cast<unsigned>(inputStream.tellg());
	inputStream.seekg(0, std::ios::beg);

	//printf("Reading %u bytes\n", numBytes);
	char* data = new(std::nothrow) char[numBytes];
	assert(data);
	std::fill(data, data + numBytes - 1, 0);
	inputStream.read(data, numBytes);
	inputStream.close();

	m_nodeData = reinterpret_cast<PathingNodes*>(data);
	char* dataOffset = data + sizeof(PathingNodes);
	m_nodes = reinterpret_cast<Node*>(dataOffset);
	dataOffset += sizeof(m_nodes[0]) * m_nodeData->numNodes;
	m_links = reinterpret_cast<NodeLink*>(dataOffset);

	return true;
}

void NodeMap::Dump()
{
	for (unsigned j = 0; j < m_nodeData->numNodes; ++j)
	{
		printf("Node[%2d] = (%7.2f, %7.2f, %7.2f), id = %2u\n", j,
			m_nodes[j].position.x,
			m_nodes[j].position.y,
			m_nodes[j].position.z,
			m_nodes[j].id
			);
	}
	printf("----\n");
	for (unsigned j = 0; j < m_nodeData->numLinks; ++j)
	{
		printf("Link[%2d] : to = %2u, from = %2u, cost = %7.2f\n", j,
			m_links[j].to,
			m_links[j].from,
			m_links[j].cost
			);
	}
	printf("----\n");
}