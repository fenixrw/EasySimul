#include "Service.h"
#include <iostream>

Service::Service(RandomController *r) : SimulationNode(r)
{
	finishServiceTime = -1;//idle
	lastServiceTime = 0;// used to calculate idle time
	entityInService = NULL;
	totalIdleTime = 0;
	timeLimit = 0;
	secondaryRandControl = NULL;
	startTurn = -1;
	endTurn = -1;
	simulation = 1;
	entityOutgoingCounter = 0;
	entityIncomingCounter = 0;
}

Service::~Service()
{
}

unsigned long Service::getEntityIncomingCounter()
{
	return entityIncomingCounter;
}

unsigned long Service::getEntityOutgoingCounter()
{
	return entityOutgoingCounter;
}

unsigned long long Service::getTotalIdleTime()
{
	return totalIdleTime;
}

unsigned long Service::getID()
{
	return id;
}

void Service::restart()
{
	finishServiceTime = -1;//idle
	lastServiceTime = 0;// used to calculate idle time
	entityInService = NULL;
}

void Service::setOutputQueue(Queue *out)
{
	primaryOutputQueue = out;
}

void Service::setSecondaryService(RandomController *r, Queue *out, unsigned long tLimit)
{
	secondaryRandControl = r;
	secondaryOutputQueue = out;
	timeLimit = tLimit;
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

	entityInService = (EntityS*)entryQueue->getNext(currentTime);

	if (entityInService!=NULL)
	{
		unsigned long long serviceTime = randControl->getRandom();
#ifdef OUTPUT_DEBUG_RAND
		std::cout << serviceTime << std::endl;
#endif

		entityInService->enterService(currentTime, id);
		entityIncomingCounter++;

		outputQueue = primaryOutputQueue;
		if (finishServiceTime < 0)
		{
			long long idleTime = currentTime - lastServiceTime;
			//std::cout << "idleTime: " << idleTime << std::endl;
			totalIdleTime += idleTime;
		}
		
		finishServiceTime = currentTime + serviceTime;
		lastServiceTime = finishServiceTime;
	}
	else
	{
		if (timeLimit > currentTime &&(secondaryRandControl != NULL && secondaryOutputQueue != NULL))
		{
			unsigned long long serviceTime = secondaryRandControl->getRandom();		
#ifdef OUTPUT_DEBUG_RAND
			std::cout << serviceTime << std::endl;
#endif
			outputQueue = secondaryOutputQueue;
			entityInService = new OutgoingEntity();

			entityInService->enterService(currentTime, id);
			entityOutgoingCounter++;

			//TODO: set Entity parameters
			finishServiceTime = currentTime + serviceTime;
			lastServiceTime = finishServiceTime;
		}
		else
		{
			finishServiceTime = -1; //idle
		}
	}
}

long long Service::getNextTime(unsigned long long currentTime)
{
	if (finishServiceTime < 0)
	{
		long long cTime = currentTime;
		if (startTurn > -1 && startTurn > cTime)
		{
			//std::cout <<  "(ST)";
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
		//std::cout << "Client Processed by the Service" << std::endl;
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

	long long idleTime = cTime - lastServiceTime;
	//std::cout << "idleTime: " << idleTime << std::endl;
	if (idleTime>0)
		totalIdleTime += idleTime;

	simulation++;
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