#include "meshes.h"

void Mesh::GenNormals()
{
	/* For every vertex */
	for (unsigned int x = 0; x < vertexCount; x++)
	{
		/* Find in how many triangles it is */
		unsigned int cnt = 0;
		for (unsigned int y = 0; y < indexCount; y++)
		{
			if (indices[y].x == x || indices[y].y == x || indices[y].z == x)
				cnt++;
		}

		/* Make a list of the triangles */
		Vec3ui *t = new Vec3ui[cnt];
		cnt = 0;
		for (unsigned int y = 0; y < indexCount; y++)
		{
			if (indices[y].x == x || indices[y].y == x || indices[y].z == x)
			{
				t[cnt] = indices[y];
				cnt++;
			}
		}

		/* Calculate the normal */
		vertices[x * 2 + 1] = Vec3();
		for (unsigned int y = 0; y < cnt; y++)
		{
			Vec3 a = vertices[t[y].x * 2];
			Vec3 b = vertices[t[y].y * 2];
			Vec3 c = vertices[t[y].z * 2];

			Vec3 ab, ac;

			if (t[y].x == x)
			{
				ab = a - b;
				ac = a - c;
			}

			if (t[y].y == x)
			{
				ab = b - a;
				ac = b - c;
			}

			if (t[y].z == x)
			{
				ab = c - a;
				ac = c - b;
			}

			vertices[x * 2 + 1] = vertices[x * 2 + 1] + cross(ab, ac);
		}
		vertices[x * 2 + 1] = vertices[x * 2 + 1] / cnt;

		delete t;
	}
}

Mesh& Mesh::operator+=(const Mesh& right)
{
	/* Merge vertices */
	unsigned int tVertexCount = this->vertexCount + right.vertexCount;
	Vec3 *tVertices = new Vec3[tVertexCount * 2];

	for (unsigned int i = 0; i < this->vertexCount * 2; i++)
	{
		tVertices[i] = this->vertices[i];
	}
	for (unsigned int i = 0; i < right.vertexCount * 2; i++)
	{
		tVertices[(this->vertexCount * 2) + i] = right.vertices[i];
	}

	/* Merge indices */
	unsigned int tIndexCount = this->indexCount + right.indexCount;
	Vec3ui *tIndices = new Vec3ui[tIndexCount];

	for (unsigned int i = 0; i < this->indexCount; i++)
	{
		tIndices[i] = this->indices[i];
	}
	for (unsigned int i = 0; i < right.indexCount; i++)
	{
		tIndices[this->indexCount + i] = right.indices[i] + this->vertexCount;
	}

	/* Merge texture coordinates */
	unsigned int tTexCount = this->texCoordsCount + right.texCoordsCount;
	Vec2 *tTexCoords = new Vec2[tTexCount];

	for (unsigned int i = 0; i < this->texCoordsCount; i++)
	{
		tTexCoords[i] = this->texCoords[i];
	}
	for (unsigned int i = 0; i < right.texCoordsCount; i++)
	{
		tTexCoords[this->texCoordsCount + i] = right.texCoords[i];
	}

	/* Delete old vertices and indices */
	if (this->vertices != NULL) delete this->vertices;
	if (this->indices != NULL) delete this->indices;
	if (this->texCoords != NULL) delete this->texCoords;

	/* Update mesh */
	this->vertexCount = tVertexCount;
	this->vertices = tVertices;
	this->indexCount = tIndexCount;
	this->indices = tIndices;
	this->texCoordsCount = tTexCount;
	this->texCoords = tTexCoords;

	return *this;
}

Mesh::Mesh() 
{
}

Mesh::Mesh(unsigned int vCount, unsigned int iCount) 
{
	this->vertexCount = vCount;
	this->vertices = new Vec3[vCount * 2];

	this->indexCount = iCount;
	this->indices = new Vec3ui[iCount];
}

Mesh::~Mesh()
{
	if (vertices != NULL) delete vertices;
	if (indices != NULL) delete indices;
	if (texCoords != NULL) delete texCoords;
}

Vec3 *genQuadVertices(float scaleX, float scaleY) 
{
	Vec3 *t = new Vec3[4];
	float x = 1.f * scaleX;
	float y = 1.f * scaleY;
	t[0] = Vec3(-x, y, 0.f);
	t[1] = Vec3(-x, -y, 0.f);
	t[2] = Vec3(x, y, 0.f);
	t[3] = Vec3(x, -y, 0.f);
	return t;
}

Vec2 bezierCurve(Spline *c, float t)
{
	float x = powf(1 - t, 3) * c->p0.x + powf(1 - t, 2) * 3 * t * c->p1.x + (1 - t) * 3 * t * t * c->p2.x + t * t * t * c->p3.x;
	float y = powf(1 - t, 3) * c->p0.y + powf(1 - t, 2) * 3 * t * c->p1.y + (1 - t) * 3 * t * t * c->p2.y + t * t * t * c->p3.y;
	return Vec2(x, y);
}

void generateMeshCurve(Vec3 **vertices, Vec3ui **indices, Spline *c, float radius, unsigned int steps)
{
	float t = 0.f;
	float stepT = 1.f / (steps + 1);
	double stepXY = (2 * M_PI) / 8;

	/* Generate vertices */
	*vertices = new Vec3[(2 + steps * 8)*2];

	Vec2 point1 = bezierCurve(c, t);
	Vec2 point2 = bezierCurve(c, 1.f);
	(*vertices)[0] = { point1.x, point1.y, 0.f };
	(*vertices)[1] = { point2.x, point2.y, 0.f };

	for (unsigned int m = 0; m < steps; m++) 
	{
		double cycle = 0.0;
		t += stepT;
		
		point1 = bezierCurve(c, t - stepT);
		point2 = bezierCurve(c, t + stepT);
		double angleA = atan2(point2.y - point1.y, point2.x - point1.x);

		point1 = bezierCurve(c, 1 - (t + stepT));
		point2 = bezierCurve(c, 1 - (t - stepT));
		double angleB = atan2(point2.y - point1.y, point2.x - point1.x);

		point1 = bezierCurve(c, t);
		point2 = bezierCurve(c, 1 - t);
		for (unsigned int i = 0; i < 8; i++)
		{
			Vec2 pointT;

			pointT.x = point1.x - sin(angleA) * point1.y;
			pointT.y = point1.y + cos(angleA) * (sin(cycle)*radius);
			(*vertices)[(2 + m * 8 + i) * 2 - 2] = { pointT.x, pointT.y, (float)cos(cycle)*radius };
			
			pointT.x = point2.x - sin(angleA) * point2.y;
			pointT.y = point2.y + cos(angleA) * (sin(cycle)*radius);
			(*vertices)[(2 + m * 8 + i) * 2 - 1] = { pointT.x, pointT.y, (float)cos(cycle)*radius };
			cycle += stepXY;
		}
	}

	point1 = bezierCurve(c, 1.f);
	point2 = bezierCurve(c, 0.f);
	(*vertices)[(steps * 8 + 2)*2 - 2] = { point1.x, point1.y, 0.f };
	(*vertices)[(steps * 8 + 2)*2 - 1] = { point2.x, point2.y, 0.f };

	/* Generate indices */
	*indices = new Vec3ui[2 * 8 + (steps - 1) * 8 * 2];
	sphericalIndicesGeneration(*indices, steps, 8);
}

unsigned int sphereVertices(unsigned int parallels, unsigned int meridians)
{
	return 2 + parallels * meridians;
}

unsigned int sphereIndices(unsigned int parallels, unsigned int meridians)
{
	return 2 * meridians + (parallels - 1) * meridians * 2;
}

void genSphere(Vec3 **vertices, Vec3ui **indices, unsigned int parallels, unsigned int meridians)
{
	if (parallels < 1) { return; }
	if (meridians < 3) { return; }

	*vertices = new Vec3[2 + parallels * meridians];

	/* Generates vertices */
	double z = 1.0;

	double stepZ = 2.0 / (parallels + 1);
	double stepXY = 2 * M_PI / meridians;

	(*vertices)[0] = { 0, 0, 1 };
	for (unsigned int i = 0; i < parallels; i++) 
	{
		z = z - stepZ;
		double angle = asin(z);
		float radius = (float)cos(angle);
		double cycle = 0;
		for (unsigned int m = 0; m < meridians; m++) 
		{
			(*vertices)[i * meridians + m + 1] = { (float)sin(cycle)*radius, (float)cos(cycle)*radius, (float)z };
			cycle += stepXY;
		}
	}
	(*vertices)[parallels*meridians + 1] = { 0, 0, -1 };

	/* Generates indices */
	*indices = new Vec3ui[2 * meridians + (parallels - 1) * meridians * 2];
	sphericalIndicesGeneration(*indices, parallels, meridians);
}

void sphereTexCoords(Vec2 **coords, unsigned int parallels, unsigned int meridians)
{
	unsigned int a = 0;
	if (parallels % 2 != 0) a++;

	*coords = new Vec2[sphereVertices(parallels, meridians)];

	double z = 1.0;
	double stepZ = 2.0 / (parallels + 1);

	(*coords)[0] = Vec2(0.5f, 0.5f);

	Vec2 prevP = Vec2((float)cos(asin(z)), (float)z);
	float *l = new float[parallels];
	for (unsigned int i = 0; i < (parallels + a) / 2; i++)
	{
		z = z - stepZ;
		double angle = asin(z);
		float radius = (float)cos(angle);
		Vec2 currP = Vec2((float)radius, (float)z);
		Vec2 vec = Vec2(currP.x - prevP.x, currP.y - prevP.y);
		l[i] = sqrtf(vec.x * vec.x + vec.y * vec.y);
		prevP = currP;
	}

	float ol = 0.f;
	for (unsigned int i = 0; i < (parallels + a) / 2; i++) 
	{
		ol += l[i];
	}

	for (unsigned int i = 1; i < (parallels + a) / 2; i++) 
	{
		l[i] += l[i - 1];
	}

	for (unsigned int i = 0; i < parallels / 2; i++) 
	{
		l[parallels - i - 1] = l[i];
	}

	double stepXY = 2 * M_PI / meridians;
	for (unsigned int i = 0; i < parallels; i++)
	{
		l[i] = l[i] / ol * 0.5f;
		double cycle = 0.0;
		for (unsigned int m = 0; m < meridians; m++) 
		{
			(*coords)[i * meridians + m + 1] = { 0.5f + (float)sin(cycle)*l[i], 0.5f + (float)cos(cycle)*l[i] };
			cycle += stepXY;
		}
	}

	(*coords)[parallels*meridians + 1] = Vec2(0.5f, 0.5f);

	delete l;
}

void sphericalIndicesGeneration(Vec3ui *indices, unsigned int parallels, unsigned int meridians)
{
	unsigned int position = 0;
	unsigned int start = 0;
	unsigned int help = 0;

	for (unsigned int i = 0; i < meridians; i++)
	{
		if (i + 1 < meridians)
		{
			help = i + 2;
		}
		else 
		{
			help = start + 1;
		}
		indices[position] = Vec3ui(start, i + 1, help);
		position++;
	}

	for (unsigned int i = 0; i < parallels - 1; i++)
	{
		unsigned int origin = 1 + meridians * i;

		unsigned int temp = origin;
		help = origin + 1;
		for (unsigned int m = 0; m < meridians; m++) 
		{

			unsigned int down = temp + meridians;
			unsigned int smth = down + 1;

			if (m + 1 == meridians) 
			{
				help = origin;
				smth = origin + meridians;
			}

			indices[position] = Vec3ui(temp, down, help);
			indices[position + 1] = Vec3ui(help, down, smth);

			temp++;
			help++;
			position += 2;
		}
	}

	start = 1 + (parallels - 1) * meridians;
	unsigned int temp = start;
	help = start;
	unsigned int end = 1 + parallels * meridians;
	for (unsigned int i = 0; i < meridians; i++)
	{
		if (i + 1 < meridians)
		{
			help++;
		}
		else
		{
			help = start;
		}
		indices[position] = Vec3ui(temp , end, help);
		temp++;
		position++;
	}

}