#pragma once
#include "types.h"

/* CoordSystem struct */
struct Coord
{
	unsigned int depth;
	Vec3 root;
	Vec3 axes[3];
	//unsigned int nB = 0;
	unsigned int nC = 0;
	Coord *coords = NULL;


	Coord();
	Coord(Coord *parent, Vec3 offset);
};

/* CoordSystem Stack structs */
struct ItemStackCoordS
{
	ItemStackCoordS *next = NULL;
	Coord *content = NULL;

	ItemStackCoordS();
	ItemStackCoordS(Coord *c);
};

class StackCoordS {
private:
	ItemStackCoordS *head = NULL;

public:
	StackCoordS();
	~StackCoordS();

	int Empty();
	Coord *Pop();
	void Push(Coord *c);
};