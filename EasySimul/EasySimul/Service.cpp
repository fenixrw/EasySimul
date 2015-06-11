#include "Service.h"


Service::Service(RandomController *r) : SimulationNode(r)
{
	finishServiceTime = -1;//idle
	lastServiceTime = 0;// used to calculate idle time
	entityInService = NULL;
}


Service::~Service()
{
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
	if (entryQueue->getNext(entityInService))
	{
		unsigned long long serviceTime = randControl->getRandom();
		unsigned long long idleTime = currentTime - lastServiceTime;

		finishServiceTime = currentTime + serviceTime;
		lastServiceTime = finishServiceTime;
	}
	else
	{
		finishServiceTime = -1; //idle
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
		outputQueue->add(entityInService);
		getNextEntity(currentTime);
	}
}

void Service::end(unsigned long long currentTime)
{

}