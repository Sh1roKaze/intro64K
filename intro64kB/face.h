#pragma once

#include "types.h"
#include "meshes.h"

struct Face
{
	Vec3 point[3];
	Vec3 normal;
	Vec2 texCoord[3];

	void GenNormal();
	void GenTexCoords();
	void InvertAxis(int axis);
};

struct FacesListItem
{
	FacesListItem(Face *f);
	FacesListItem *next;
	Face *face;
};

class FacesList 
{
public:
	~FacesList();

	void Insert(Vec3 a, Vec3 b, Vec3 c);
	void Insert(Face *f);
	
	void MirrorByAxis(int axis);
	Mesh *GenerateMesh();


private:
	FacesListItem *head = NULL;
	FacesListItem *curr = NULL;
	unsigned int length = 0;
};