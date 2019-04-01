#pragma once
#include "Nodes/Nodes.h"
#include "AStarPath.h"

#define MAX_NUM_LIST 150

class PathFinding
{
private:
	//KeyValuePairRipOff neighbors[MAX_NUM_LIST];
	AStarPath open[MAX_NUM_LIST];
	AStarPath closed[MAX_NUM_LIST];
	AStarPath parents[MAX_NUM_LIST];
	Node returnPath[MAX_NUM_LIST];
	AStarPath current;
	int count = 0;
	int openCount = 0;
	int parentCount = 0;
	Node target;
public:
	unsigned maxNodes;
	unsigned maxLinks;
	int getCount();
	Node* nodes;
	NodeLink* links;
	/*
	Finds all the neighbors and determines
	the fitness of each neighbor and then
	puts the one with the lowest fitness
	into the closed list.  If a neighbor
	has a lower fitness and is already in
	the closed list then put it back in 
	the open and have it become the current.
	If make sure to leave the neighbors that
	weren't touched in the open list.
	*/
	Node* FindPath(Node, Node);
	bool FindNeighbors();
};

