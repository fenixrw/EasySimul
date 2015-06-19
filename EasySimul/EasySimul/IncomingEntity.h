#pragma once
#include "EntityS.h"
class IncomingEntity :
	public EntityS
{
public:
	IncomingEntity();
	~IncomingEntity();

	virtual void enterQueue(unsigned long long currentTime, unsigned long queueID);
	virtual void exitQueue(unsigned long long currentTime, unsigned long queueID);

	static unsigned long long getMaxTimeInQueue();
	static unsigned long long getMaxTimeInQueueSum();
	unsigned long long getTotalTimeInQueue();
	
	static void restart();
	static void end();

protected:
	
	unsigned long long totalTimeInQueue;
	long long enterQueueTime;
	long idQueue;

	static unsigned long long maxTimeInQueue;
	static unsigned long long maxTimeInQueueSum;
	static unsigned long long maxTimeInQueueAtSimulation;
};

