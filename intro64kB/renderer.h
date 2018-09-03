#pragma once

#include "types.h"
#include "meshes.h"
#include "renderable.h"
#include "shaders.h"

#include <glad\glad.h> 
#include <gl\gl.h> 


struct Camera
{
	Vec3 position;
	Vec3 direction;
	Vec3 up;
	double pitch;
	double yaw;
	double roll;

	Vec3 lightPosition;

	Camera();
	void Rotate(Vec3 v);
	void move(float change);
	void point(int chX, int chY);
	void tilt(float change);
};

class Renderer
{
public:


	void UpdateProjectionMatrix(float fov, float aspectRatio, float znear, float zfar);
	void UpdateLightPosition(Vec3 newPosition);

	void UpdateEverything();

	void AddToRenderQueue(Renderable* item);
	void AddToRenderQueue(RenderableList *rl);

	void UseCamera(Camera *c);

	void Render();
	GLuint RenderToTexture(int width, int height);
	
	Renderer();
	~Renderer();

private:
	RenderQueue queue;
	Camera *camera;

	Mat4 viewMatrix;
	Mat4 projectionMatrix;
	Vec3 cameraPosition;
	Vec3 cameraDirection;
	Vec3 cameraRotation;
	Vec3 lightPosition;

	int updatedViewMatrix = 1;
	int updatedProjectionMatrix = 1;
	int updatedCameraPosition = 1;
	int updatedLightPosition = 1;

	int isCameraUpdated();
	void UpdateViewMatrix();
};