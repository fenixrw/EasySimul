#include "Service.h"
#include <iostream>
unsigned long Service::idController = 0;

Service::Service(RandomController *r)
{
	randControl = r;
	finishServiceTime = -1;//idle
	lastServiceTime = 0;// used to calculate idle time
	entityInService = NULL;
	idleTime = 0;
	simulationTime = 0;
	timeLimit = 0;
	startTurn = 0;
	endTurn = -1;
	entityCounter = 0;
	id = idController;
	idController++;
}

Service::~Service()
{
}

unsigned long long Service::getSimulationTime()
{
	return simulationTime;
}

unsigned long Service::getEntityCounter()
{
	return entityCounter;
}

unsigned long long Service::getIdleTime()
{
	return idleTime;
}

unsigned long Service::getID()
{
	return id;
}

void Service::restart()
{
	idleTime = 0;
	simulationTime = 0;
	entityCounter = 0;

	finishServiceTime = -1;//idle
	lastServiceTime = 0;// used to calculate idle time
	if (startTurn > 0)
	{
		lastServiceTime = startTurn;
	}
	entityInService = NULL;
}

void Service::setOutputQueue(Queue *out)
{
	outputQueue = out;
}

void Service::setInputQueue(Queue *in)
{
	entryQueue = in;
}

void Service::getNextEntity(unsigned long long currentTime)
{
	long long cTime = currentTime;

	if ((startTurn > -1 && startTurn > cTime) || (endTurn > -1 && endTurn < cTime))
	{
		finishServiceTime = -1;
		return;
	}

	entityInService = entryQueue->getNext(currentTime);

	if (entityInService!=NULL)
	{
		unsigned long long serviceTime = randControl->getRandom();
#ifdef OUTPUT_DEBUG_RAND
		std::cout << serviceTime << std::endl;
#endif
		entityInService->totalTimeInService += serviceTime;
		entityInService->enterService(currentTime, id);
		entityCounter++;

		if (finishServiceTime < 0)
		{
			long long idleT = currentTime - lastServiceTime;
			idleTime += idleT;
		}
		
		finishServiceTime = currentTime + serviceTime;
		lastServiceTime = finishServiceTime;
	}
	else
	{
		finishServiceTime = -1; //idle
	}
}

long long Service::getNextTime(unsigned long long currentTime)
{
	if (finishServiceTime < 0)
	{
		long long cTime = currentTime;
		if (startTurn > -1 && startTurn > cTime)
		{
			return startTurn;
		}

		getNextEntity(currentTime);
	}
	return finishServiceTime;
}

void Service::update(unsigned long long currentTime)
{
	if (finishServiceTime == currentTime)
	{
		if (entityInService != NULL)
		{
			entityInService->exitService(currentTime, id);
			outputQueue->add(entityInService, currentTime);
		}
		else
		{
			//error
		}

		getNextEntity(currentTime);
	}
}

void Service::end(unsigned long long currentTime)
{
	long long cTime = currentTime;

	if (endTurn > -1)
		cTime = endTurn;

	long long idleT = cTime - lastServiceTime;

	if (idleT>0)
		idleTime += idleT;

	unsigned long long simulTime = cTime;
	if (startTurn > 0)
	{
		simulTime -= startTurn;
	}

	simulationTime += simulTime;
}

void Service::setTurn(long long startT, long long endT)
{
	startTurn = startT;
	if (startTurn > 0)
	{
		lastServiceTime = startTurn;// used to calculate idle time
	}

	endTurn = endT;
}