#pragma once
#include <cstdlib>

#include "types.h"
#include "meshes.h"
#include "coord.h"
#include "face.h"

#define JOINT_HEIGHT 0.3f
#define WING_HEIGHT 0.15f

#define SPLINE_SEGMENTS 32

enum RuleType
{
	ARRAY_UNIFORM,
	ARRAY_VARRYING,
	ARRAY_CASCADE
};

/* LSystem class */
class LSystem
{
public:
	LSystem(int seed);
	~LSystem();

	Mesh *GetSpaceship();

private:
	Coord *top = NULL;
	FacesList *list = NULL;
	
	void SpaceshipRulesApply();
	void ApplyArrayRule(RuleType rule, Coord *parent, int axis);

	void GenHullMesh(Coord *c);
	void GenJointMesh(Coord *c);
	void GenEngineMesh(Coord *c);
	void GenWingMesh(Coord *c);
};
