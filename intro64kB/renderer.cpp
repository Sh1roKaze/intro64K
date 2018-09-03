#include "renderer.h"



Camera::Camera() 
{
	position = Vec3(0.f, 0.f, 0.f);
	direction = Vec3(0.f, 0.f, 1.f);
	up = Vec3(0.f, 1.f, 0.f);
	pitch = 0.0;
	yaw = 0.0;
	roll = 0.0;
	lightPosition = Vec3();
};

void Camera::Rotate(Vec3 v)
{
	double alpha = v.x;
	double beta = v.y;
	double gamma = v.z;

	Mat4 rotationMatrix;
	rotationMatrix.col[0] = Vec4(cos(beta), sin(beta)*sin(gamma), sin(beta)*cos(gamma), 0.f);
	rotationMatrix.col[1] = Vec4(sin(beta)*sin(alpha), cos(gamma) * cos(alpha) - cos(beta) * sin(gamma) * sin(alpha), -cos(alpha) * sin(gamma) - cos(beta) * cos(gamma) * sin(alpha), 0.f);
	rotationMatrix.col[2] = Vec4(-sin(beta) * cos(alpha), cos(gamma) * sin(alpha) + cos(beta) * cos(alpha) * sin(gamma), -sin(gamma) * sin(alpha) + cos(beta) * cos(gamma) * cos(alpha), 0.f);
	rotationMatrix.col[3] = Vec4(0.f, 0.f, 0.f, 1.f);

	direction = rotationMatrix * direction;
	up = rotationMatrix * up;

	/*if (direction.z > 0.f)
		up.y = 1.f;
	else
		up.y = -1.f;*/
}


void Camera::move(float change) 
{
	position = position + (direction * change);
}

void Camera::point(int chX, int chY)
{
	pitch -= chX * 0.01;
	if (pitch > 6.28318530718)
	{
		pitch -= 6.28318530718;
	}
	if (pitch < 0.f)
	{
		pitch += 6.28318530718;
	}

	yaw += chY * 0.01;
	if (yaw > 6.28318530718)
	{
		yaw -= 6.28318530718;
	}
	if (yaw < 0.f) 
	{
		yaw += 6.28318530718;
	}

	direction = Vec3((float)sin(pitch), (float)sin(yaw), (float)(cos(pitch)));
	direction = normalize(direction);
}

void Camera::tilt(float change) 
{
	roll += change;
	if (roll > 6.28318530718)
	{
		roll -= 6.28318530718;
	}
	if (roll < 0.f) 
	{
		roll += 6.28318530718;
	}

	up = Vec3((float)sin(roll), (float)cos(roll), 0);
}

Renderer::Renderer()
{
}

void Renderer::UpdateViewMatrix() 
{

	Vec3 right = cross(cameraDirection, cameraRotation);
	right = normalize(right);

	Vec3 v = cross(cameraDirection, right);
	v = normalize(v);

	viewMatrix.col[0] = Vec4(right.x, v.x, cameraDirection.x, 0.f);
	viewMatrix.col[1] = Vec4(right.y, v.y, cameraDirection.y, 0.f);
	viewMatrix.col[2] = Vec4(right.z, v.z, cameraDirection.z, 0.f);
	viewMatrix.col[3] = Vec4(dot(right, cameraPosition), dot(v, cameraPosition), dot(cameraDirection, cameraPosition), 1.f);

	updatedViewMatrix = 1;
}

int Renderer::isCameraUpdated()
{
	int changed = 0;
	if (camera == NULL) return 0;

	if (camera->position != cameraPosition)
	{
		updatedCameraPosition = 1;
		cameraPosition = camera->position;
		changed = 1;
	}

	if (camera->direction != cameraDirection || camera->up != cameraRotation)
	{
		cameraDirection = camera->direction;
		cameraRotation = camera->up;
		changed = 1;
	}

	if (camera->lightPosition != lightPosition)
		updatedLightPosition = 1;

	return changed;
}

void Renderer::UpdateProjectionMatrix(float fov, float aspectRatio, float znear, float zfar)
{
	float top, right, twoZNear, deltaW, deltaH, deltaZ;
	top = (float)(znear * tan(fov * M_PI / 360.0));
	right = top * aspectRatio;

	twoZNear = 2.0f * znear;
	deltaW = right - (-right);
	deltaH = top - (-top);
	deltaZ = zfar - znear;

	projectionMatrix.col[0] = Vec4(twoZNear / deltaW, 0.f, 0.f, 0.f);
	projectionMatrix.col[1] = Vec4(0.f, twoZNear / deltaH, 0.f, 0.f);
	projectionMatrix.col[2] = Vec4((right + (-right)) / deltaW, (top + (-top)) / deltaH, (-zfar - znear) / deltaZ, -1.f);
	projectionMatrix.col[3] = Vec4(0.f, 0.f, (-twoZNear * zfar) / deltaZ, 0.f);

	updatedProjectionMatrix = 1;
}

void Renderer::UpdateLightPosition(Vec3 newPosition) 
{
	lightPosition = newPosition;
	updatedLightPosition = 1;
}

void Renderer::UpdateEverything()
{
	updatedViewMatrix = 1;
	updatedProjectionMatrix = 1;
	updatedCameraPosition = 1;
	updatedLightPosition = 1;
}

void Renderer::AddToRenderQueue(Renderable* item)
{
	queue.addItem(item);
}

void Renderer::AddToRenderQueue(RenderableList *rL)
{
	Renderable *t = rL->First();
	while (t != NULL)
	{
		queue.addItem(t);
		t = rL->Next();
	}
}

void Renderer::UseCamera(Camera *c)
{
	camera = c;
	cameraPosition = c->position;
	cameraDirection = c->direction;
	cameraRotation = c->up;
	lightPosition = c->lightPosition;
	UpdateEverything();
}

void Renderer::Render() 
{
	GLint uniformLocation;

	/* Update view matrix on camera movement */
	if (isCameraUpdated() == 1)
	{
		UpdateViewMatrix();
	}

	/* Clear screen and depth buffer before rendering new frame */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* Render every item in render queue */
	while (!queue.empty())
	{
		Renderable *r = queue.pop();
		glUseProgram(r->program);

		if (updatedCameraPosition == 1) 
		{
			uniformLocation = glGetUniformLocation(r->program, "cameraPosition");
			glUniform3fv(uniformLocation, 1, (GLfloat*) &(cameraPosition));
		}

		if (updatedLightPosition == 1) 
		{
			uniformLocation = glGetUniformLocation(r->program, "lightPosition");
			glUniform3fv(uniformLocation, 1, (GLfloat*)&camera->lightPosition);
		}

		if (updatedViewMatrix == 1)
		{
			uniformLocation = glGetUniformLocation(r->program, "viewMatrix");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (GLfloat*)&viewMatrix);
		}

		if (updatedProjectionMatrix == 1) 
		{
			uniformLocation = glGetUniformLocation(r->program, "projectionMatrix");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (GLfloat*)&projectionMatrix);
		}

		uniformLocation = glGetUniformLocation(r->program, "modelMatrix");
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (GLfloat*)&(r->modelMatrix));

		uniformLocation = glGetUniformLocation(r->program, "scale");
		glUniform1f(uniformLocation, r->scale);

		glBindVertexArray(r->vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, r->texture);
		glDrawElements(GL_TRIANGLES, r->triangles * 3, GL_UNSIGNED_INT, 0);
	}

	updatedCameraPosition = 0;
	updatedLightPosition = 0;
	updatedViewMatrix = 0;
	updatedProjectionMatrix = 0;
}

GLuint Renderer::RenderToTexture(int width, int height)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum DrawBuffers = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &DrawBuffers);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Gives an empty image to OpenGL
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		return 0;

	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);	//Disables depth testing

	Render();

	glEnable(GL_DEPTH_TEST);	// Enables depth testing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//Binds default framebuffer
	glViewport(0, 0, 1280, 720);

	glDeleteBuffers(1, &DrawBuffers);
	glDeleteFramebuffers(1, &fbo);

	return texture;
}

Renderer::~Renderer() 
{

}