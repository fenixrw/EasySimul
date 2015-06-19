#include "IncomingEntity.h"

unsigned long long IncomingEntity::maxTimeInQueue = 0;
unsigned long long IncomingEntity::maxTimeInQueueSum = 0;
unsigned long long IncomingEntity::maxTimeInQueueAtSimulation = 0;

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

unsigned long long IncomingEntity::getMaxTimeInQueueSum()
{
	return maxTimeInQueueSum;
}

void IncomingEntity::restart()
{
	maxTimeInQueueAtSimulation = 0;
}

void IncomingEntity::end()
{
	maxTimeInQueueSum += maxTimeInQueueAtSimulation;
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

	if (totalTimeInQueue > maxTimeInQueueAtSimulation)
	{
		maxTimeInQueueAtSimulation = totalTimeInQueue;
	}
}