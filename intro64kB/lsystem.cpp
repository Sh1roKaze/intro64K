#include "lsystem.h"
#include <random>

LSystem::LSystem(int seed)
{
	srand(seed);
	top = new Coord();
	top->depth = 0;
	top->axes[0] = Vec3(4.0f, 0.0f, 0.0f) * ((float) rand() /RAND_MAX + 0.5f);
	top->axes[1] = Vec3(0.0f, 2.5f, 0.0f) * ((float) rand() / RAND_MAX + 0.5f);
	top->axes[2] = Vec3(0.0f, 0.0f, 1.0f) * ((float) rand() / RAND_MAX + 0.5f);
	
	SpaceshipRulesApply();
}

LSystem::~LSystem()
{
}

void LSystem::SpaceshipRulesApply()
{
	ApplyArrayRule(ARRAY_VARRYING, top, 1);
	for (unsigned int i = 0; i < top->nC; i++)
	{
		ApplyArrayRule(ARRAY_UNIFORM, &(top->coords[i]), 0);
	}
} //SpaceshipRulesApply()

void LSystem::ApplyArrayRule(RuleType rule, Coord *parent, int axis)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(4, 8);
	int sectors = (int)floor(distribution(generator));

	parent->nC = sectors;
	parent->coords = new Coord[sectors];

	if (rule == ARRAY_UNIFORM)
	{
		for (int i = 0; i < sectors; i++)
		{
			Coord subC = Coord();
			subC.depth = parent->depth + 1;
			subC.root = parent->root + ((parent->axes[axis] / sectors) * i);
			subC.axes[0] = parent->axes[0];
			subC.axes[1] = parent->axes[1];
			subC.axes[2] = parent->axes[2];
			subC.axes[axis] = subC.axes[axis] / sectors;

			parent->coords[i] = subC;
		}
	}

	if (rule == ARRAY_CASCADE)
	{
		Vec3 shrink = Vec3(length(parent->axes[0]), length(parent->axes[1]), length(parent->axes[2])) / sectors;

		for (int i = 0; i < sectors; i++)
		{
			Coord subC = Coord();
			subC.depth = parent->depth + 1;
			subC.root = parent->root + ((parent->axes[axis] / sectors) * i);
			subC.axes[0] = parent->axes[0] - ((parent->axes[0] / sectors) * i);
			subC.axes[1] = parent->axes[1] - ((parent->axes[1] / sectors) * i);
			subC.axes[2] = parent->axes[2] - ((parent->axes[2] / sectors) * i);
			subC.axes[axis] = subC.axes[axis] / sectors;

			parent->coords[i] = subC;
		}
	}

	if (rule == ARRAY_VARRYING)
	{
		Vec3 root_next = parent->root;
		Vec3 remaining_axis = parent->axes[axis];

		for (int i = 0; i < sectors; i++)
		{
			float help = (float)rand() / (float)RAND_MAX;

			Coord subC = Coord();
			subC.depth = parent->depth + 1;
			subC.root = root_next;
			subC.axes[0] = parent->axes[0];
			subC.axes[1] = parent->axes[1];
			subC.axes[2] = parent->axes[2];
			subC.axes[axis] = remaining_axis * help;

			parent->coords[i] = subC;

			root_next = root_next + remaining_axis * help;
			remaining_axis = remaining_axis - remaining_axis * help;
		}
		parent->coords[sectors - 1].axes[axis] = parent->coords[sectors - 1].axes[axis] + remaining_axis;
	}
} //ApplyArrayRule()

Mesh *LSystem::GetSpaceship()
{
	list = new FacesList();

	GenHullMesh(&(top->coords[0]));

	for (unsigned int i = 1; i < top->nC - 2; i++)
	{
		GenJointMesh(&(top->coords[i]));
	}

	GenEngineMesh(&(top->coords[top->nC - 2]));
	
	GenWingMesh(&(top->coords[top->nC - 1]));
	
	list->MirrorByAxis(1);
	list->MirrorByAxis(2);
	
	return list->GenerateMesh();
}

void LSystem::GenHullMesh(Coord *c)
{
	/* Init */
	float minY = c->root.y;
	float minZ = c->root.z + (c->axes[2].z * JOINT_HEIGHT);
	float maxY = c->root.y + c->axes[1].y;
	float maxZ = c->root.z + c->axes[2].z;

	Spline hullCurve =
	{
		Vec2(maxY, minZ),
		Vec2(maxY, (maxZ + minZ) / 2),
		Vec2(maxY / 2, maxZ),
		Vec2(minY, maxZ)
	};

	Coord *h = &(c->coords[0]);

	/* First  */
	list->Insert(
		h->root + h->axes[0],
		h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[1]);
	list->Insert(
		h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[1]);
	list->Insert(
		h->root + h->axes[1],
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[0] + h->axes[1]);
	list->Insert(
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[0] + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[0] + h->axes[1]);

	float t = 0.f;
	for (unsigned int i = 0; i < SPLINE_SEGMENTS; i++)
	{
		Vec2 p1 = bezierCurve(&hullCurve, t);
		Vec2 p2 = bezierCurve(&hullCurve, t + 1.f / (float) SPLINE_SEGMENTS);
		list->Insert(
			Vec3(h->root.x, p1.x, p1.y),
			Vec3(h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT)),
			Vec3(h->root.x, p2.x, p2.y) );
		t +=  1.f / (float) SPLINE_SEGMENTS;
	}

	/* Middle */
	for (unsigned int i = 0; i < c->nC - 1; i++)
	{
		h = &(c->coords[i]);

		t = 0.f;
		for (unsigned int i = 0; i < SPLINE_SEGMENTS; i++)
		{
			Vec2 p1 = bezierCurve(&hullCurve, t);
			Vec2 p2 = bezierCurve(&hullCurve, t + 1.f / (float)SPLINE_SEGMENTS);
			list->Insert(
				Vec3(h->root.x + h->axes[0].x, p1.x, p1.y),
				Vec3(h->root.x, p1.x, p1.y),
				Vec3(h->root.x, p2.x, p2.y));

			list->Insert(
				Vec3(h->root.x + h->axes[0].x, p2.x, p2.y),
				Vec3(h->root.x + h->axes[0].x, p1.x, p1.y),
				Vec3(h->root.x, p2.x, p2.y));	

			t += 1.f / (float)SPLINE_SEGMENTS;
		}
	}

	/* Last */
	h = &(c->coords[c->nC - 1]);
	t = 0.f;
	for (unsigned int i = 0; i < SPLINE_SEGMENTS; i++)
	{
		Vec2 p1 = bezierCurve(&hullCurve, t);
		Vec2 p2 = bezierCurve(&hullCurve, t + 1.f / (float)SPLINE_SEGMENTS);
		list->Insert(
			Vec3(h->root.x, p1.x, p1.y),
			Vec3(h->root.x, p2.x, p2.y),
			Vec3(h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT)) );
		t += 1.f / (float)SPLINE_SEGMENTS;
	}

	list->Insert(
		h->root + h->axes[1],
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT) );

	list->Insert(
		h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[0],
		h->root + h->axes[1] );
} //GenHullMesh

void LSystem::GenJointMesh(Coord *c)
{
	Coord *h = &(c->coords[1]);

	list->Insert(
		h->root,
		h->root + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[1] );

	list->Insert(
		h->root + h->axes[1],
		h->root + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT) );

	for (unsigned int i = 1; i < c->nC - 1; i++)
	{
		h = &(c->coords[i]);

		list->Insert(
			h->root + (h->axes[2] * JOINT_HEIGHT),
			h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
			h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT) );

		list->Insert(
			h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
			h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
			h->root + h->axes[0] + h->axes[1] + (h->axes[2] * JOINT_HEIGHT) );
	}

	h = &(c->coords[c->nC - 1]);
	list->Insert(
		h->root,
		h->root + h->axes[1],
		h->root + (h->axes[2] * JOINT_HEIGHT) );

	list->Insert(
		h->root + h->axes[1],
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + (h->axes[2] * JOINT_HEIGHT) );
} //GenJointMesh

void LSystem::GenEngineMesh(Coord *c)
{
	/* Init */
	float xBegin, xEnd;

	float minY = c->root.y;
	float minZ = c->root.z + (c->axes[2].z * JOINT_HEIGHT);
	float maxY = c->root.y + c->axes[1].y;
	float maxZ = c->root.z + c->axes[2].z;

	Spline spl =
	{
		Vec2(minY, minZ),
		Vec2(minY, maxZ),
		Vec2(maxY, maxZ),
		Vec2(maxY, minZ)
	};

	Coord *h = &(c->coords[0]);
	Vec3 s = h->root + h->axes[0] * 0.8f;

	/* JOINT SIDE */
	list->Insert(
		s + h->axes[0],
		s + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
		s );
	list->Insert(
		s + h->axes[0   ] + (h->axes[2] * JOINT_HEIGHT),
		s + (h->axes[2] * JOINT_HEIGHT),
		s );

	/* OTHER JOINT/WING SIDE */
	list->Insert(
		s + h->axes[1],
		s + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[0] + h->axes[1]);
	list->Insert(
		s + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[0] + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[0] + h->axes[1]);

	/* ANGLED INWARDS */
	list->Insert(
		s + h->axes[0] + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[0] + (h->axes[1] / 2),
		s);
	list->Insert(
		s + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[0] + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		s);


	list->Insert(
		s + h->axes[0] + (h->axes[1] / 2),
		s + h->axes[0] + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[1]);
	list->Insert(
		s + h->axes[0] + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		s + h->axes[1]); 
	
	float t = 0.f;
	for (unsigned int i = 0; i < SPLINE_SEGMENTS; i++)
	{
		Vec2 p1 = bezierCurve(&spl, t);
		Vec2 p2 = bezierCurve(&spl, t + 1.f / (float)SPLINE_SEGMENTS);
		list->Insert(
			Vec3(s + h->axes[0] + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT)),
			Vec3(s.x, p1.x, p1.y),
			Vec3(s.x, p2.x, p2.y));
		t += 1.f / (float)SPLINE_SEGMENTS;
	}

	/* ENGINE HULL */
	t = 0.f;
	for (unsigned int x = 0; x < c->nC; x++)
	{
		h = &(c->coords[x]);

		xBegin = 0.f;
		xEnd = 1.0f;
		if (x == 0) xBegin = 0.8f;
		if (x == c->nC - 1) xEnd = 0.2f;

		float b = (h->root + (h->axes[0] * xBegin)).x;
		float e = (h->root + (h->axes[0] * xEnd)).x;

		t = 0.f;
		for (unsigned int i = 0; i < SPLINE_SEGMENTS; i++)
		{
			Vec2 p1 = bezierCurve(&spl, t);
			Vec2 p2 = bezierCurve(&spl, t + 1.f / (float)SPLINE_SEGMENTS);

			list->Insert(
				Vec3(b, p1.x, p1.y),
				Vec3(e, p1.x, p1.y),
				Vec3(b, p2.x, p2.y));

			list->Insert(
				Vec3(e, p1.x, p1.y),
				Vec3(e, p2.x, p2.y),
				Vec3(b, p2.x, p2.y));

			t += 1.f / (float)SPLINE_SEGMENTS;
		}
	}

	/* FRONT ENGINE SIDE */
	Vec3 stop = h->root + (h->axes[0] * 0.2);
	/* JOINT SIDE */
	list->Insert(
		stop,
		stop + (h->axes[2] * JOINT_HEIGHT),
		h->root);
	list->Insert(
		stop + (h->axes[2] * JOINT_HEIGHT),
		h->root + (h->axes[2] * JOINT_HEIGHT),
		h->root);

	/* OTHER JOINT/WING SIDE */
	list->Insert(
		stop + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		stop + h->axes[1],
		h->root + h->axes[1]);
	list->Insert(
		h->root + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		stop + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + h->axes[1]);

	/* ANGLED INWARDS */
	list->Insert(
		h->root + (h->axes[1] / 2),
		h->root + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		stop);
	list->Insert(
		h->root + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		stop + (h->axes[2] * JOINT_HEIGHT),
		stop);

	list->Insert(
		h->root + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		h->root + (h->axes[1] / 2),
		stop + h->axes[1]);
	list->Insert(
		stop + h->axes[1] + (h->axes[2] * JOINT_HEIGHT),
		h->root + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT),
		stop + h->axes[1]);

	t = 0.f;
	for (unsigned int i = 0; i < SPLINE_SEGMENTS; i++)
	{
		Vec2 p1 = bezierCurve(&spl, t);
		Vec2 p2 = bezierCurve(&spl, t + 1.f / (float)SPLINE_SEGMENTS);
		list->Insert(
			Vec3(stop.x, p1.x, p1.y),
			Vec3(h->root + (h->axes[1] / 2) + (h->axes[2] * JOINT_HEIGHT)),
			Vec3(stop.x, p2.x, p2.y));
		t += 1.f / (float)SPLINE_SEGMENTS;
	}
} //GenEngineMesh

void LSystem::GenWingMesh(Coord *c)
{
	/* Init */
	float minX = c->root.x;
	float minY = c->root.y;
	float maxX = c->root.x + c->axes[0].x;
	float maxY = c->root.y + c->axes[1].y;

	Spline spl =
	{
		Vec2(minX, minY),
		Vec2(minX, maxY * 2),
		Vec2(maxX/2, maxY),
		Vec2(maxX, minY)
	};
	
	float t = 1.f / (float)c->nC;
	for (unsigned int i = 1; i < c->nC - 1; i++)
	{
		Coord *h = &(c->coords[i]);

		Vec2 p1 = bezierCurve(&spl, t);
		Vec2 p2 = bezierCurve(&spl, t + 1.f / (float)c->nC);

		if (i == 1)
		{
			list->Insert(
				h->root,
				h->root + h->axes[2] * JOINT_HEIGHT,
				Vec3(p1.x, p1.y, h->root.z + h->axes[2].z * WING_HEIGHT) );

			list->Insert(
				Vec3(p1.x, p1.y, h->root.z + h->axes[2].z * WING_HEIGHT),
				Vec3(p1.x, p1.y, h->root.z),
				h->root );
		}

		if (i == c->nC - 2)
		{
			list->Insert(
				Vec3(p2.x, p2.y, h->root.z),
				Vec3(p2.x, p2.y, h->root.z + h->axes[2].z * WING_HEIGHT),
				h->root + h->axes[0] );

			list->Insert(
				h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
				h->root + h->axes[0],
				Vec3(p2.x, p2.y, h->root.z + h->axes[2].z * WING_HEIGHT) );
		}

		list->Insert(
			h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
			Vec3(p1.x, p1.y, h->root.z + h->axes[2].z * WING_HEIGHT),
			h->root + (h->axes[2] * JOINT_HEIGHT) );

		list->Insert(
			h->root + h->axes[0] + (h->axes[2] * JOINT_HEIGHT),
			Vec3(p2.x, p2.y, h->root.z + h->axes[2].z * WING_HEIGHT),
			Vec3(p1.x, p1.y, h->root.z + h->axes[2].z * WING_HEIGHT) );

		list->Insert(
			Vec3(p1.x, p1.y, h->root.z + h->axes[2].z * WING_HEIGHT),
			Vec3(p2.x, p2.y, h->root.z + h->axes[2].z * WING_HEIGHT),
			Vec3(p2.x, p2.y, h->root.z) );

		list->Insert(
			Vec3(p1.x, p1.y, h->root.z),
			Vec3(p1.x, p1.y, h->root.z + h->axes[2].z * WING_HEIGHT),
			Vec3(p2.x, p2.y, h->root.z) 
		);
		t += 1.f / (float)c->nC;
	}
} //GenWingMesh
