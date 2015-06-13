#pragma once
#include "SimulationNode.h"

class Service : public SimulationNode
{
public:
	Service(RandomController *r);
	~Service();

	virtual long long getNextTime(unsigned long long currentTime);
	virtual void update(unsigned long long currentTime);
	virtual void end(unsigned long long currentTime);
	virtual void restart();


	void setOutputQueue(Queue *out);
	void setInputQueue(Queue *in);
	void setSecondaryService(RandomController *r, Queue *out, unsigned long tLimit);

	unsigned long long totalIdleTime;

	void setTurn(long long startT, long long endT);

protected:

	void getNextEntity(unsigned long long currentTime);
	unsigned long simulation; /*Simulation Day/Number */
	unsigned long timeLimit;
	long long finishServiceTime; /*-1 = idle (None Entity)*/
	long long lastServiceTime; /*starts at 0*/
	long long startTurn;
	long long endTurn;
	Entity* entityInService;
	Queue* entryQueue;
	Queue* outputQueue;
	Queue * primaryOutputQueue;
	Queue * secondaryOutputQueue;
	RandomController *secondaryRandControl;
};

