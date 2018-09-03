#pragma once

#include <functional>

#include "midi.h"
#include "renderer.h"
#include "types.h"

enum EventStatus 
{
	ACTIVE,
	FINISHED
};

class Event
{
public:
	EventStatus Status = ACTIVE;

	double Start;
	double End;

	Camera *camera = NULL;
	Renderable *renderable = NULL;
	Vec3 vector = NULL;

	std::function<void(Camera* c, Renderable* r, Vec3 v, double t)> EventFunction;

	Event();
	Event(double s, double e);
	~Event();
};

struct EventListItem
{
	EventListItem *next;
	Event *e;

	EventListItem();
	EventListItem(Event *newEvent);
	~EventListItem();
};

class EventList
{
public:
	EventList();
	EventList(Event *e);
	~EventList();

	void Insert(Event *e);

	Event *First();
	Event *Next();

private:
	EventListItem * head = NULL;
	EventListItem *curr = NULL;
};

namespace EventFunctions
{
	extern HWND hWnd;

	void RotateBy(Camera *c, Renderable *r, Vec3 v, double t);
	void RotateTo(Camera *c, Renderable *r, Vec3 v, double t);


	void MoveBy(Camera *c, Renderable *r, Vec3 v, double t);
	void MoveTo(Camera *c, Renderable *r, Vec3 v, double t);
	
	void MoveLightBy(Camera *c, Renderable *r, Vec3 v, double t);
	void MoveLightTo(Camera *c, Renderable *r, Vec3 v, double t);

	void SetScale(Camera *c, Renderable *r, Vec3 v, double t);

	void PlayMusic(Camera *c, Renderable *r, Vec3 v, double t);
}