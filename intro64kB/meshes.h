#pragma once

#include "types.h"

struct Mesh 
{
	unsigned int vertexCount = 0;
	Vec3 *vertices = NULL;

	unsigned int indexCount = 0;
	Vec3ui *indices = NULL;

	unsigned int texCoordsCount = 0;
	Vec2 *texCoords = NULL;

	void GenNormals();

	Mesh& operator+=(const Mesh& right);

	Mesh();
	Mesh(unsigned int vCount, unsigned int iCount);
	~Mesh();
};

struct Spline 
{
	Vec2 p0;
	Vec2 p1;
	Vec2 p2;
	Vec2 p3;
};

Vec3 *genQuadVertices(float scaleX, float scaleY);

Vec2 bezierCurve(Spline *c, float t);
void generateMeshCurve(Vec3 **vertices, Vec3ui **indices, Spline *c, float radius, unsigned int steps);

unsigned int sphereVertices(unsigned int parallels, unsigned int meridians);
unsigned int sphereIndices(unsigned int parallels, unsigned int meridians);
void genSphere(Vec3 **vertices, Vec3ui **indices, unsigned int parallels, unsigned int meridians);
void sphereTexCoords(Vec2 **coords, unsigned int parallels, unsigned int meridians);

void sphericalIndicesGeneration(Vec3ui *indices, unsigned int parallels, unsigned int meridians);