#include "Service.h"
#include <iostream>
unsigned int Service::minFreeServices = 1;
unsigned int Service::freeServices = 0;
unsigned long Service::idController = 0;

Service::Service(RandomController *r) : SimulationNode(r)
{
	finishServiceTime = -1;//idle
	lastServiceTime = 0;// used to calculate idle time
	entityInService = NULL;
	totalIdleTime = 0;
	totalSimulationTime = 0;
	timeLimit = 0;
	secondaryRandControl = NULL;
	startTurn = 0;
	endTurn = -1;
	simulation = 1;
	entityOutgoingCounter = 0;
	entityIncomingCounter = 0;
	freeIncomingServiceRule = false;
	freeIncomingServiceRuleStarted = false;
	id = idController;
	idController++;
}

Service::~Service()
{
}

void Service::activeFreeIncomingServiceRule()
{
	freeIncomingServiceRule = true;
}

unsigned long long Service::getTotalSimulationTime()
{
	return totalSimulationTime;
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
	freeServices = 0;
	freeIncomingServiceRuleStarted = false;
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
		if (freeIncomingServiceRule && freeIncomingServiceRuleStarted)
		{
			if (endTurn > -1 && endTurn < cTime)
			{
				freeServices--;
				freeIncomingServiceRuleStarted = false;
			}
		}

		finishServiceTime = -1;
		return;
	}
	else if (freeIncomingServiceRule)
	{
		if (cTime == startTurn)
		{
			freeIncomingServiceRuleStarted = true;
			freeServices++;
		}
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

		if (freeIncomingServiceRule)
		{
			freeServices--;
		}
	}
	else
	{
		if (timeLimit > currentTime &&(secondaryRandControl != NULL && secondaryOutputQueue != NULL))
		{
			if (freeIncomingServiceRule)
			{
				if(freeServices>minFreeServices)
				{
					freeServices--;
				}
				else
				{
					finishServiceTime = -1;
					return;
				}
			}

			unsigned long long serviceTime = secondaryRandControl->getRandom();		
#ifdef OUTPUT_DEBUG_RAND
			std::cout << serviceTime << std::endl;
#endif
			outputQueue = secondaryOutputQueue;
			entityInService = new OutgoingEntity();

			entityInService->enterSystem(currentTime);
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
			if (freeIncomingServiceRule)
			{
				freeServices++;
			}

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

	unsigned long long simulTime = cTime;
	if (startTurn > 0)
	{
		simulTime -= startTurn;
	}

	totalSimulationTime += simulTime;

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