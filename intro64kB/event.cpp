#include "event.h"

Event::Event()
{
}

Event::Event(double s, double e)
{
	Start = s;
	End = e;
}

Event::~Event()
{
}

EventListItem::EventListItem()
{
}

EventListItem::EventListItem(Event *newEvent)
{
	e = newEvent;
	next = NULL;
}


EventListItem::~EventListItem()
{
}

EventList::EventList()
{
}

EventList::EventList(Event *e)
{
	Insert(e);
}

EventList::~EventList() 
{
}

void EventList::Insert(Event *e)
{
	if (curr == NULL)
	{
		head = new EventListItem(e);
		curr = head;
	}
	else
	{
		EventListItem *t = curr->next;
		curr->next = new EventListItem(e);
		curr = curr->next;
		curr->next = t;
	}
}

Event *EventList::First()
{
	this->curr = this->head;
	if (this->curr == NULL)
		return NULL;
	return this->head->e;
}

Event *EventList::Next()
{
	if (curr != NULL && curr->next != NULL)
	{
		curr = curr->next;
		return curr->e;
	}
	return NULL;
};

namespace EventFunctions 
{
	HWND hWnd;

	void MoveBy(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (c != NULL)
		{
			c->position = c->position + (v * t);
		}

		if (r != NULL)
		{
			r->position = r->position + (v * t);
			r->updateModelMatrix();
		}
	}

	void MoveTo(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (c != NULL)
		{
			c->position = v;
		}

		if (r != NULL)
		{
			r->position = v;
			r->updateModelMatrix();
		}
	}

	void RotateBy(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (c != NULL)
		{
			c->Rotate(v * t);
		}

		if (r != NULL)
		{
			r->rotation = r->rotation + (v * t);
			r->updateModelMatrix();
		}
	}

	void RotateTo(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (c != NULL)
		{
			c->direction = Vec3(0, 0, 1.f);
			c->Rotate(v);
		}

		if (r != NULL)
		{
			r->rotation = v;
			r->updateModelMatrix();
		}
	}

	void MoveLightBy(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (c != NULL)
		{
			c->lightPosition = c->lightPosition + (v * t);
		}
	}

	void MoveLightTo(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (c != NULL)
		{
			c->lightPosition = v;
		}
	}

	void SetScale(Camera *c, Renderable *r, Vec3 v, double t)
	{
		if (r != NULL)
		{
			r->scale = v.x;
		}
	}

	void PlayMusic(Camera *c, Renderable *r, Vec3 v, double t)
	{
		char *tempPath = initMIDIfile();
		DWORD midiRes = playMIDIFile(hWnd, tempPath);
		delete tempPath;
		//if (midiRes != 0L) { return 45; }
	}
}
