#include "coord.h"

Coord::Coord()
{
	root = Vec3(0.f);
};

Coord::Coord(Coord *parent, Vec3 offset)
{
	root = parent->root + offset;
	axes[0] = parent->axes[0];
	axes[1] = parent->axes[1];
	axes[2] = parent->axes[2];
}

ItemStackCoordS::ItemStackCoordS()
{
}

ItemStackCoordS::ItemStackCoordS(Coord *c)
{
	content = c;
}

StackCoordS::StackCoordS()
{
}

StackCoordS::~StackCoordS()
{
}

int StackCoordS::Empty()
{
	if (head == NULL)
		return 1;
	return 0;
}

Coord *StackCoordS::Pop()
{
	if (head != NULL) 
	{
		Coord *t = head->content;
		ItemStackCoordS *d = head;
		head = head->next;
		delete d;
		return t;
	}
	else
	{
		return NULL;
	}
}

void StackCoordS::Push(Coord *c)
{
	ItemStackCoordS *t = new ItemStackCoordS(c);
	t->next = head;
	head = t;
}