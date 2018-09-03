#include "face.h"

void Face::GenNormal()
{
	normal = normalize(cross(point[1] - point[0], point[2] - point[0]));
}

void Face::GenTexCoords()
{
	texCoord[0] = Vec2(0.0, 0.0);
	texCoord[1] = Vec2(1.0, 0.0);
	texCoord[2] = Vec2(0.0, 1.0);
}

void Face::InvertAxis(int axis)
{
	if (axis == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			point[i] = Vec3(point[i].x * -1.f, point[i].y, point[i].z);
		}
		normal = Vec3(normal.x * -1.f, normal.y, normal.z);
	}

	if (axis == 1)
	{
		for (int i = 0; i < 3; i++)
		{
			point[i] = Vec3(point[i].x, point[i].y * -1.f, point[i].z);
		}
		normal = Vec3(normal.x, normal.y * -1.f, normal.z);
	}

	if (axis == 2)
	{
		for (int i = 0; i < 3; i++)
		{
			point[i] = Vec3(point[i].x, point[i].y, point[i].z * -1.f);
		}
		normal = Vec3(normal.x, normal.y, normal.z * -1.f);
	}
}

FacesListItem::FacesListItem(Face *f)
{
	face = f;
	next = NULL;
}

FacesList::~FacesList()
{
	if (head == NULL) return;
	FacesListItem* next = head;
	while (next != NULL)
	{
		FacesListItem *temp = next;
		next = next->next;

		delete temp;
	}
}

void FacesList::Insert(Vec3 a, Vec3 b, Vec3 c)
{
	Face *f = new Face();
	f->point[0] = a;
	f->point[1] = b;
	f->point[2] = c;
	f->GenNormal();
	f->GenTexCoords();
	Insert(f);
}

void FacesList::Insert(Face *f)
{
	if (curr == NULL)
	{
		head = new FacesListItem(f);
		curr = head;
	}
	else
	{
		curr->next = new FacesListItem(f);
		curr = curr->next;
	}
	length++;
};

void FacesList::MirrorByAxis(int axis)
{
	if (head == NULL) return;


	Face *f = head->face;
	Face *nf = new Face();
	*nf = *f;
	nf->InvertAxis(axis);
	curr->next = new FacesListItem(nf);
	curr = curr->next;
	length++;

	FacesListItem *stop = curr;
	for (FacesListItem *temp = head->next; temp != stop; temp = temp->next)
	{
		Face *f = temp->face;
		Face *nf = new Face();

		*nf = *f;
		nf->InvertAxis(axis);

		curr->next = new FacesListItem(nf);
		curr = curr->next;
		length++;
	}
}

Mesh *FacesList::GenerateMesh()
{
	if (head == NULL) return NULL;

	Mesh *m = new Mesh(length * 3, length);
	
	m->texCoordsCount = length * 3;
	m->texCoords = new Vec2[m->texCoordsCount];

	FacesListItem* next = head;
	for (unsigned int i = 0; i < length; i++)
	{
		m->vertices[i * 6 + 0] = next->face->point[0];
		m->vertices[i * 6 + 1] = next->face->normal;
		m->vertices[i * 6 + 2] = next->face->point[1];
		m->vertices[i * 6 + 3] = next->face->normal;
		m->vertices[i * 6 + 4] = next->face->point[2];
		m->vertices[i * 6 + 5] = next->face->normal;

		m->indices[i] = Vec3ui(1, 0, 2) + (i * 3);

		m->texCoords[(i * 3) + 0] = next->face->texCoord[0];
		m->texCoords[(i * 3) + 1] = next->face->texCoord[1];
		m->texCoords[(i * 3) + 2] = next->face->texCoord[2];

		next = next->next;
	}

	return m;
};