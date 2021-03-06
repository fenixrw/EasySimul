#include "Queue.h"

unsigned long Queue::idController = 0;

Queue::Queue(QUEUE_TYPE t)
{
	type = t;
	id = idController;
	idController++;
	maxSize = 0;
	lastOperationTime = 0;
}

Queue::~Queue()
{
}

void Queue::end(unsigned long long currentTime)
{
	unsigned long size = entityQueue.size();

	if (timePerSize.find(size) != timePerSize.end())
	{
		timePerSize[size] += currentTime - lastOperationTime;
	}
	else
	{
		timePerSize[size] = currentTime - lastOperationTime;
	}

	lastOperationTime = currentTime;

	//TODO: if !ExitQueue --> Erase queue & save info if clients were lost
}

void Queue::restart()
{
	maxSize = 0;
	lastOperationTime = 0;
}

std::vector<Entity*> Queue::emptyToVector()
{
	std::vector<Entity*> v;

	while (!entityQueue.empty())
	{
		v.push_back(entityQueue.front());
		entityQueue.pop();
	}

	return v;
}

std::map<unsigned long, unsigned long long> Queue::getTimePerSize()
{
	return timePerSize;
}

unsigned long Queue::getMaxSize()
{
	return maxSize;
}

unsigned long Queue::getID()
{
	return id;
}

int Queue::getSize()
{
	return entityQueue.size();
}

Entity* Queue::getNext(unsigned long long currentTime)
{
	Entity *e = NULL;
	if (entityQueue.empty())
	{
		return e;
	}

	e = entityQueue.front();

	switch (type)
	{
	case NormalQueue:
		e->totalTimeInQueue += (currentTime - e->enterQueueTime);
		e->exitQueue(currentTime, id);
		break;
	case ExitQueue:
		break;
	case EntryQueue:
		e->totalTimeInQueue += (currentTime - e->enterQueueTime);
		e->exitQueue(currentTime, id);
		break;
	default:
		break;
	}

	unsigned long size = entityQueue.size();

	if (timePerSize.find(size) != timePerSize.end())
	{
		timePerSize[size] += currentTime - lastOperationTime;
	}
	else
	{
		timePerSize[size] = currentTime - lastOperationTime;
	}

	lastOperationTime = currentTime;
	
	entityQueue.pop();


	return e;
}

bool Queue::add(Entity *e, unsigned long long currentTime)
{
	if (e == NULL)
	{
		return false;
	}

	switch (type)
	{
	case NormalQueue:
		e->enterQueueTime = currentTime;
		e->enterQueue(currentTime,id);
		break;
	case ExitQueue:
		e->exitSystemTime = currentTime;
		e->exitSystem(currentTime);
		break;
	case EntryQueue:
		e->enterQueueTime = currentTime;
		e->enterSystemTime = currentTime;
		e->enterSystem(currentTime);
		e->enterQueue(currentTime, id);
		break;
	default:
		break;
	}

	unsigned long size = entityQueue.size();

	if (timePerSize.find(size) != timePerSize.end())
	{
		timePerSize[size] += currentTime - lastOperationTime;
	}
	else
	{
		timePerSize[size] = currentTime - lastOperationTime;
	}

	lastOperationTime = currentTime;

	entityQueue.push(e);

	//Update max size if needed
	if (entityQueue.size() > maxSize)
	{
		maxSize = entityQueue.size();
	}

	return true;
}