#include "Generator.h"


Generator::Generator(RandomController *r) : SimulationNode(r)
{
	nextEntityArrival = randControl->getRandom();
	timeLimit = 0;
}


Generator::~Generator()
{
}


long long Generator::getNexTime(unsigned long long currentTime)
{
	return nextEntityArrival;
}

void Generator::update(unsigned long long currentTime)
{
	if (nextEntityArrival < timeLimit)
	{
		if (currentTime == nextEntityArrival)
		{
			Entity *e = new Entity();
			outputQueue->add(e);
			nextEntityArrival = currentTime + randControl->getRandom();
		}
	}
	else
	{
		nextEntityArrival = -1;
	}
}

void Generator::end(unsigned long long currentTime)
{
	
}


void Generator::setOutputQueue(Queue *out)
{
	outputQueue = out;
}


void Generator::setTimeLimit(unsigned long tLimit)
{
	timeLimit = tLimit;
}