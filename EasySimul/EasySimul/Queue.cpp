#include "Queue.h"


Queue::Queue()
{
}


Queue::~Queue()
{
}

int Queue::getSize()
{
	return entityQueue.size();
}

bool Queue::getNext(Entity *e)
{
	if (entityQueue.empty())
	{
		e = NULL;
		return false;
	}

	e = entityQueue.front();

	entityQueue.pop();

	return true;
}

bool Queue::add(Entity *e)
{
	entityQueue.push(e);
	return true;
}