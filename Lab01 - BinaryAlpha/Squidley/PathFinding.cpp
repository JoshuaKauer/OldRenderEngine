#include "PathFinding.h"

int PathFinding::getCount()
{
	return count;
}

Node* PathFinding::FindPath(Node start, Node end)
{
	if (start.id == end.id)
	{
		return nullptr;
	}
	Node blank;
	AStarPath blank2;
	for (int j = 0; j < MAX_NUM_LIST; ++j)
	{
		open[j] = blank2;
		closed[j] = blank2;
		returnPath[j] = blank;
	}
	count = 0;
	openCount = 0;
	parentCount = 0;
	target = end;
	current.g = 0;
	current.fitness = glm::distance(start.position, end.position);
	current.current = start;
	current.parent = nullptr;
	//Stop when lowest rank in open is end node
	while (current.current.id != end.id)
	{
		//Move current to closed list
		closed[count++] = current;
		if (count >= MAX_NUM_LIST)
			return nullptr;
		parents[parentCount] = current;
		//Find all the neighbors
		if (!FindNeighbors())
			return nullptr;
	}
	

	Node path[MAX_NUM_LIST];
	int pathCount = 0;
	do
	{
		path[pathCount++] = current.current;
		if (pathCount >= MAX_NUM_LIST)
			return nullptr;
		current = *current.parent;
	} while (current.parent);
	//Reconstruct reverse path from goal to start by following the parents
	path[pathCount] = current.current;

	//Then go back through one more time in the correct order
	for (int j = 0; j <= pathCount; ++j)
	{
		returnPath[j] = path[pathCount - j];
	}
	return returnPath;
}

bool PathFinding::FindNeighbors()
{
	//From the current node use the links and find where it has the same Node ID in the from as the current node
	for (unsigned j = 0; j < maxLinks; ++j)
	{
		if (parents[parentCount].current.id == links[j].from)
		{
			float cost = parents[parentCount].g + links[j].cost;
			//If neighbor in open and has cost less than g(neighbor) remove neighbor from open, because new path is better
			for (int i = 0; i <= openCount; ++i)
			{
				if (open[i].current.id == links[j].to && cost < open[i].g)
				{
					for (int x = i; x < openCount; ++x)
					{
						open[x] = open[x + 1];
					}
					--openCount;
				}
			}
			//If neighbor in closed and cost less than g(neighbor) remove neighbor from closed
			for (int i = 0; i <= count; ++i)
			{
				if (closed[i].current.id == links[j].to && cost < closed[i].g)
				{
					for (int x = i; x < count; ++x)
					{
						closed[x] = closed[x + 1];
					}
					--count;
				}
			}
			AStarPath next;
			next.g = cost;
			next.fitness = next.g + glm::distance(nodes[links[j].to].position, target.position);
			next.parent = &parents[parentCount];
			next.current = nodes[links[j].to];
			open[openCount++] = next;
			if (openCount >= MAX_NUM_LIST)
				return false;
		}
	}
	++parentCount;
	if (parentCount >= MAX_NUM_LIST)
		return false;
	if (openCount == 0)
		return false;
	//Set Lowest fitness to current with parent of now previous
	//If two or more have same fitness pick first one that shows up
	AStarPath newCur = open[0];
	for (int j = 0; j < openCount; ++j)
	{
		if (open[j].fitness < newCur.fitness)
			newCur = open[j];
	}
	this->current = newCur;
	for (int i = 0; i <= openCount; ++i)
	{
		if (open[i].current.id == newCur.current.id)
		{
			for (int x = i; x < openCount; ++x)
			{
				open[x] = open[x + 1];
			}
			--openCount;
		}
	}
	return true;
}
