	/* OLD stuff */
	/*
	void genSquare(Vec3 center, float l, Vec3 *arr) 
	{
		arr[0] = Vec3(center.x + l/2, center.y + l/2, center.z);
		arr[1] = Vec3(center.x + l/2, center.y - l/2, center.z);
		arr[2] = Vec3(center.x - l/2, center.y + l/2, center.z);
		arr[3] = Vec3(center.x - l/2, center.y - l/2, center.z);
	}


	struct Mesh {
		Vec3 *vertices;
		Vec3ui *indices;
	};

	Mesh *createSpaceship(int seed) {
		Mesh *spaceS = new Mesh();

		LSystem *lsys = new LSystem(seed);

		spaceS->vertices = lsys->GenVertices();
		spaceS->indices = lsys->GenIndices();

		return spaceS;
	}*/

	/*
	class CoordList : Content {
		//std::vector<Coords>
	};

	class Coords {
	public:
		Vec3 position;
		Vec3 basis[3];
		Content content;
	};

	std::vector<Coords>arrayRule(Coords const&c, size_t n, site_t axis) {
		std::vector<Coords>result;
		for (size_t i = 0; i<n; ++i) {
			Coords subC;
			subC.position = c.position + basis[axis] / float(n*i);
			subC.basis = subC.basis;
			subC.basis[axis] /= n;
			result.push_back(subC)
		}
		return result;
	}

	void createBox(Coords const&c) {

	}
	*/