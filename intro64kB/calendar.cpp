#include "calendar.h"


Scene::Scene()
{
	rList = new RenderableList();
	eventList = new EventList();
}

Scene::Scene(double s, double e) : Scene::Scene()
{
	Start = s;
	End = e;
}

Scene::~Scene()
{
	delete eventList;
	delete rList;
}

RenderableList *Scene::GetRenderables()
{
	return rList;
};

void Scene::AddRenderable(Renderable *r)
{
	rList->Insert(r);
};

void Scene::AddEvent(Event *e)
{
	eventList->Insert(e);
};

void Scene::UpdateState(double timeBefore, double timeNow)
{
	double t1 = timeBefore;
	double t2 = timeNow;

	Event *e = eventList->First();
	while (e != NULL)
	{
		if (e->Start > timeNow || e->Status == FINISHED) 
		{
			e = eventList->Next();
			continue;
		}
		if (timeBefore < e->Start)  t1 = e->Start;
		if (timeNow > e->End) t2 = e->End;

		double t = (t2 - t1) / (e->End - e->Start);
		if (t > 0.00000000)
			e->EventFunction(e->camera, e->renderable, e->vector, t);

		if (e->End < timeNow)
			e->Status = FINISHED;

		e = eventList->Next();
	}
}

void Scene::Reset()
{
	Event *t = eventList->First();
	while (t != NULL)
	{
		t->Status = ACTIVE;
		t = eventList->Next();
	}
}

Calendar::Calendar()
{
	currentScene = 0;
	scenes = new Scene*[MAX_SCENES];
	clockLast = std::chrono::high_resolution_clock::now();
	timeNow = 0.f;
}

Calendar::~Calendar()
{
}

void Calendar::Reset()
{
	if (timeNow < scenes[MAX_SCENES - 1]->End) return;

	for (int i = 0; i < MAX_SCENES; i++)
	{
		scenes[i]->Reset();
	}
	timeNow = 0.f;
	currentScene = 0;
	activeScene = scenes[0];
	clockLast = std::chrono::high_resolution_clock::now();
};

void Calendar::Tick()
{

	if (activeScene == NULL) return;
 
	double timeBefore = timeNow;

	clockNow = std::chrono::high_resolution_clock::now();
	
	int64_t a = std::chrono::duration_cast<std::chrono::microseconds>(clockNow - clockLast).count();
	//if (a == 0) return;

	timeNow +=  ((double)a)/1000;
	
	if (timeNow > activeScene->End && currentScene < MAX_SCENES - 1)
	{
		activeScene = scenes[currentScene + 1];
		currentScene++;
	}

	activeScene->UpdateState(timeBefore, timeNow);

	clockLast = clockNow;
};

void Calendar::AddScene(Scene *scene)
{
	if (addSceneCnt == 0)
		activeScene = scene;

	scenes[addSceneCnt] = scene;
	addSceneCnt++;
};

Camera *Calendar::GetCamera()
{
	return NULL;
};
RenderableList *Calendar::GetRenderables()
{
	return activeScene->GetRenderables();
};


