#pragma once
#include "SimulationNode.h"

class Service : public SimulationNode
{
public:
	Service(RandomController *r);
	~Service();

	virtual long long getNexTime(unsigned long long currentTime);
	virtual void update(unsigned long long currentTime);
	virtual void end(unsigned long long currentTime);


	void setOutputQueue(Queue *out);
	void setInputQueue(Queue *in);

protected:

	void getNextEntity(unsigned long long currentTime);

	long long finishServiceTime; /*-1 = idle (None Entity)*/
	long long lastServiceTime; /*starts at 0*/
	Queue* entryQueue;
	Queue* outputQueue;
	Entity* entityInService;
};

