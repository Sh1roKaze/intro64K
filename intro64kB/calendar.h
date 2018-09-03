#pragma once
#include <chrono>

#include "renderer.h"
#include "event.h"

#define MAX_SCENES 4

class Scene
{
public:
	double Start;
	double End;


	Scene();
	Scene(double s, double e);
	~Scene();

	RenderableList *GetRenderables();

	void AddRenderable(Renderable *r);
	void AddEvent(Event *e);

	void UpdateState(double timeBefore, double timeNow);

	void Reset();

private:
	RenderableList *rList = NULL;
	EventList *eventList = NULL;
};


class Calendar
{
public:
	Calendar();
	~Calendar();

	void Reset();

	void Tick();

	void AddScene(Scene *scene);

	Camera *GetCamera();
	RenderableList *GetRenderables();
	

private:
	//double timeBefore;
	double timeNow;
	std::chrono::time_point<std::chrono::steady_clock> clockLast;
	std::chrono::time_point<std::chrono::steady_clock> clockNow;

	
	//unsigned int noS;
	unsigned int currentScene = 0;
	unsigned int addSceneCnt = 0;
	Scene **scenes = NULL;
	Scene *activeScene = NULL;
};
