#include "Entity.h"

unsigned long Entity::idController = 0;

Entity::Entity()
{
	id = idController;
	idController++;

	enterSystemTime = 0;
	exitSystemTime = 0;

	totalTimeInService = 0;
	totalTimeInQueue = 0;
	enterQueueTime = 0;
}

Entity::~Entity()
{
}


unsigned long long Entity::getTotalTimeInSystem()
{
	return exitSystemTime - enterSystemTime;
}

unsigned long Entity::getID()
{
	return id;
}

unsigned long long Entity::getTotalTimeInService()
{
	return totalTimeInService;
}

unsigned long long Entity::getTotalTimeInQueue()
{
	return totalTimeInQueue;
}