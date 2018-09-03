#include "renderable.h"

GLuint libShV = 0;
GLuint libShF = 0;

Renderable::Renderable(MeshType t)
{
	type = t;
	glGenVertexArrays(1, &vao);
}

void Renderable::createVBO(GLuint count, Vec3 *verts)
{
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (type == MESH_STATIC_NORMALS)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * 2 * count, verts, GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * count, verts, GL_STATIC_DRAW);
	}

	points = count;
}

void Renderable::createVBO(GLuint count, Vec3 *verts, Vec2 *coords)
{
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (type == MESH_STATIC_NORMALS)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * count * 2 + sizeof(Vec2) * count, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * count * 2, verts);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vec3) * count * 2, sizeof(Vec2) * count, coords);
	}
	else 
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * count + sizeof(Vec2) * count, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * count, verts);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vec3) * count, sizeof(Vec2) * count, coords);	
	}

	points = count;
}

void Renderable::createEAB(GLuint count, Vec3ui *indices)
{
	glBindVertexArray(vao);
	glGenBuffers(1, &eab);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vec3ui) * count, indices, GL_STATIC_DRAW);

	triangles = count;
}

void Renderable::createProgram(GLuint vShader, GLuint fShader)
{
	if (program != 0)
		glDeleteProgram(program);

	if (libShV == 0)
		libShV = shaderCreate(library_shader, GL_VERTEX_SHADER);
	if (libShF == 0)
		libShF = shaderCreate(library_shader, GL_FRAGMENT_SHADER);

	vertShader = vShader;
	fragShader = fShader;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);

	program = glCreateProgram();
	glAttachShader(program, libShV);
	glAttachShader(program, libShF);
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glBindFragDataLocation(program, 0, "outColor");
	glLinkProgram(program);
	glUseProgram(program);

	GLint samplerLocation = glGetUniformLocation(program, "texture_sampler");
	glUniform1i(samplerLocation, 0);

	if (type == MESH_STATIC)
	{
		GLint posAttrib = glGetAttribLocation(program, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttrib);

		GLint texCoordAttrib = glGetAttribLocation(program, "TexCoord");
		glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(Vec3) * points));
		glEnableVertexAttribArray(texCoordAttrib);
	}

	if (type == MESH_STATIC_NORMALS)
	{
		GLint posAttrib = glGetAttribLocation(program, "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3) * 2, 0);
		glEnableVertexAttribArray(posAttrib);

		GLint normalAttrib = glGetAttribLocation(program, "normal");
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3) * 2, (void*)sizeof(Vec3));
		glEnableVertexAttribArray(normalAttrib);

		GLint texCoordAttrib = glGetAttribLocation(program, "TexCoord");
		glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(Vec3) * points * 2));
		glEnableVertexAttribArray(texCoordAttrib);
	}
}

void Renderable::useTexture(GLuint tex)
{
	texture = tex;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderable::updateModelMatrix()
{
	double alpha = rotation.x;
	double beta = rotation.y;
	double gamma = rotation.z;

	modelMatrix.col[0] = Vec4(cos(beta), sin(beta)*sin(gamma), sin(beta)*cos(gamma), 0.f);
	modelMatrix.col[1] = Vec4(sin(beta)*sin(alpha), cos(gamma) * cos(alpha) - cos(beta) * sin(gamma) * sin(alpha), -cos(alpha) * sin(gamma) - cos(beta) * cos(gamma) * sin(alpha), 0.f);
	modelMatrix.col[2] = Vec4(-sin(beta) * cos(alpha), cos(gamma) * sin(alpha) + cos(beta) * cos(alpha) * sin(gamma), -sin(gamma) * sin(alpha) + cos(beta) * cos(gamma) * cos(alpha), 0.f);
	modelMatrix.col[3] = Vec4(position.x, position.y, position.z, 1.f);
}

Renderable::~Renderable()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &eab);

	glDeleteVertexArrays(1, &vao);

	glDetachShader(program, libShV);
	glDetachShader(program, libShF);
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glDeleteProgram(program);
}

QueueItem::QueueItem(Renderable *item)
{
	r = item;
}

void RenderQueue::addItem(Renderable *item)
{
	if (rear == NULL)
	{
		head = new QueueItem(item);
		rear = head;
	}
	else
	{
		rear->next = new QueueItem(item);
		rear = rear->next;
	}
}

Renderable* RenderQueue::pop()
{
	if (head != NULL)
	{
		Renderable *t = head->r;
		QueueItem *temp = head;
		head = head->next;
		if (head == NULL) rear = NULL;
		delete temp;
		return t;
	}
	else
	{
		return NULL;
	}
}

int RenderQueue::empty()
{
	if (head == NULL)
		return 1;
	return 0;
}

RenderableListItem::RenderableListItem(Renderable *newRenderable)
{
	r = newRenderable;
}

RenderableList::RenderableList()
{
}

RenderableList::RenderableList(Renderable *e)
{
	Insert(e);
}

RenderableList::~RenderableList()
{
}

void RenderableList::Insert(Renderable *r)
{
	if (curr == NULL)
	{
		head = new RenderableListItem(r);
		curr = head;
	}
	else
	{
		RenderableListItem *t = curr->next;
		curr->next = new RenderableListItem(r);
		curr = curr->next;
		curr->next = t;
	}
}

Renderable *RenderableList::First()
{
	curr = head;
	if (curr == NULL)
		return NULL;
	return head->r;
}

Renderable *RenderableList::Next()
{
	if (curr != NULL && curr->next != NULL)
	{
		curr = curr->next;
		return curr->r;
	}
	return NULL;
};