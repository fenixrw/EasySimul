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
}

Service::~Service()
{
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
	if ((startTurn > -1 && startTurn > currentTime) || (endTurn > -1 && endTurn < currentTime))
	{
		finishServiceTime = -1;
		return;
	}

	if (entryQueue->getNext(entityInService))
	{
		unsigned long long serviceTime = randControl->getRandom();
#ifdef OUTPUT_DEBUG_RAND
		std::cout << serviceTime << std::endl;
#endif
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
			entityInService = new Entity();
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

long long Service::getNexTime(unsigned long long currentTime)
{
	if (finishServiceTime < 0)
	{
		getNextEntity(currentTime);
	}
	return finishServiceTime;
}

void Service::update(unsigned long long currentTime)
{
	if (finishServiceTime == currentTime)
	{
		//std::cout << "Client Processed by the Service" << std::endl;
		outputQueue->add(entityInService);
		getNextEntity(currentTime);
	}
}

void Service::end(unsigned long long currentTime)
{

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