#pragma once
#include "types.h"
#include "shaders.h"

#include <glad\glad.h> 
#include <gl\gl.h> 

enum MeshType
{
	MESH_STATIC,
	MESH_STATIC_NORMALS
};

struct Renderable
{
	Vec3 position = Vec3();
	Vec3 rotation = Vec3();
	GLfloat scale = 1.0f;
	Mat4 modelMatrix;

	MeshType type;
	GLuint program = 0;
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint eab = 0;
	GLuint fragShader = 0;
	GLuint vertShader = 0;
	GLuint texture = 0;
	GLuint triangles = 0;
	GLuint points = 0;
	GLfloat time = 0.f;

	Renderable(MeshType t);
	~Renderable();

	void createVBO(GLuint count, Vec3 *verts);
	void createVBO(GLuint count, Vec3 *verts, Vec2 *coords);
	void createEAB(GLuint count, Vec3ui *indices);
	void createProgram(GLuint vShader, GLuint fShader);
	void useTexture(GLuint tex);
	void updateModelMatrix();
};

struct QueueItem
{
	Renderable *r;
	QueueItem *next = NULL;

	QueueItem(Renderable *item);
};

struct RenderQueue
{
private:
	QueueItem * head = NULL;
	QueueItem *rear = NULL;

public:
	void addItem(Renderable *item);
	Renderable* pop();
	int empty();
};

struct RenderableListItem
{
	RenderableListItem *next = NULL;
	Renderable *r = NULL;

	RenderableListItem(Renderable *newRenderable);
};

class RenderableList
{
public:
	RenderableList();
	RenderableList(Renderable *r);
	~RenderableList();

	void Insert(Renderable *r);

	Renderable *First();
	Renderable *Next();

private:
	RenderableListItem * head = NULL;
	RenderableListItem *curr = NULL;
};