#pragma once
#include "SimulationNode.h"

class Generator : public SimulationNode
{
public:
	Generator(RandomController *r);
	~Generator();

	virtual long long getNexTime(unsigned long long currentTime);
	virtual void update(unsigned long long currentTime);
	virtual void end(unsigned long long currentTime);

	void setTimeLimit(unsigned long tLimit);
	void setOutputQueue(Queue *out);

protected:

	unsigned long timeLimit;
	long long nextEntityArrival;
	Queue *outputQueue;
};

