#include "AStarComponent.h"
#include "SeekBehaviorComponent.h"
#include "../Logger.h"
#include "../ECS/Components/SpatialComponent.h"

bool AStarComponent::setPath(Node start, Node end)
{
	done = false;
	target = end;
	pathCount = 0;
	SeekBehavior* seek = GetSiblingComponent<SeekBehavior>();
	if (!seek)
	{
		target = start;
		done = true;
		std::string msg(this->GetName());
		msg += " can't find seek sibling";
		LOG(Error, msg.c_str());
		return false;
	}
	path = m_pathFinder.FindPath(start, end);
	if(!path)
	{
		target = start;
		done = true;
		seek->setTargetPos(&target.position);
		std::string msg(this->GetName());
		msg += " No Path Found";
		LOG(Warning, msg.c_str());
		return false;
	}
	seek->setTargetPos(&path[pathCount].position);
	return true;
}

bool AStarComponent::Update(float)
{
	SeekBehavior* seek = GetSiblingComponent<SeekBehavior>();
	if (!seek)
	{
		std::string msg(this->GetName());
		msg += " can't find seek sibling";
		LOG(Error, msg.c_str());
		return false;
	}
	SpatialComponent* space = GetSiblingComponent<SpatialComponent>();
	if (!space)
	{
		std::string msg(this->GetName());
		msg += " can't find spatial sibling";
		LOG(Error, msg.c_str());
		return false;
	}
	if (path)
	{
		if (glm::distance(space->m_position, target.position) <= 0.1f && !done)
		{
			done = true;
			seek->setTargetPos(&space->m_position);
		}
		if (glm::distance(space->m_position, path[pathCount].position) <= 0.1f && path[pathCount].id != target.id && !done)
		{
			seek->setTargetPos(&path[++pathCount].position);
		}
		return true;
	}
	return false;
}

void AStarComponent::setNodeMap(NodeMap* map)
{
	m_Map = map;
	m_pathFinder.maxNodes = map->m_nodeData->numNodes;
	m_pathFinder.maxLinks = map->m_nodeData->numLinks;
	m_pathFinder.nodes = map->m_nodes;
	m_pathFinder.links = map->m_links;
}
