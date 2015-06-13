#include "Generator.h"


Generator::Generator(RandomController *r) : SimulationNode(r)
{
	nextEntityArrival = randControl->getRandom();

#ifdef OUTPUT_DEBUG_RAND
	std::cout << nextEntityArrival << std::endl;
#endif

	timeLimit = 0;
	entityCounter = 0;
	simulation = 1;
}


Generator::~Generator()
{
}

void Generator::restart()
{
	nextEntityArrival = randControl->getRandom();
}

long long Generator::getNextTime(unsigned long long currentTime)
{
	return nextEntityArrival;
}

void Generator::update(unsigned long long currentTime)
{
	if (/*timeLimit==0 ||*/ nextEntityArrival < timeLimit)
	{
		if (currentTime == nextEntityArrival)
		{
			entityCounter++;
			Entity *e = new Entity();
			outputQueue->add(e);
			unsigned long next = randControl->getRandom();
			nextEntityArrival = currentTime + next;

#ifdef OUTPUT_DEBUG_RAND
			std::cout << next << std::endl;
#endif
		}
	}
	else
	{
		nextEntityArrival = -1;
	}
}

void Generator::end(unsigned long long currentTime)
{
	simulation++;
}


void Generator::setOutputQueue(Queue *out)
{
	outputQueue = out;
}


void Generator::setTimeLimit(unsigned long tLimit)
{
	timeLimit = tLimit;
}