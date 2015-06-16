#include "IncomingEntity.h"

unsigned long long IncomingEntity::maxTimeInQueue = 0;

IncomingEntity::IncomingEntity()
{
	enterSystemTime = 0;
	exitSystemTime = 0;
	totalTimeInQueue = 0;
	enterQueueTime = -1;
	idQueue = -1;
}


IncomingEntity::~IncomingEntity()
{
}

unsigned long long IncomingEntity::getTotalTimeInQueue()
{
	return totalTimeInQueue;
}

unsigned long long IncomingEntity::getMaxTimeInQueue()
{
	return maxTimeInQueue;
}


void IncomingEntity::enterQueue(unsigned long long currentTime, unsigned long queueID)
{
	idQueue = queueID;
	enterQueueTime = currentTime;
}

void IncomingEntity::exitQueue(unsigned long long currentTime, unsigned long queueID)
{
	totalTimeInQueue = currentTime - enterQueueTime;

	if (totalTimeInQueue > maxTimeInQueue)
	{
		maxTimeInQueue = totalTimeInQueue;
	}
}