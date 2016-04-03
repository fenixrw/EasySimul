#pragma once
#include "SimulationNode.h"
#include "Entity.h"

class Generator : public SimulationNode
{
public:
	Generator(RandomController *r);
	~Generator();

	virtual long long getNextTime(unsigned long long currentTime);
	virtual void update(unsigned long long currentTime);
	virtual void end(unsigned long long currentTime);
	virtual void restart();

	void setTimeLimit(unsigned long tLimit);
	void setOutputQueue(Queue *out);

	unsigned long entityCounter;

protected:

	RandomController *randControl;

	unsigned long timeLimit;
	long long nextEntityArrival;
	Queue *outputQueue;
};

