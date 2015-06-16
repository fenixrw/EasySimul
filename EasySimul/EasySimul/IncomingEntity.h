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
	unsigned long long getTotalTimeInQueue();


protected:
	
	unsigned long long totalTimeInQueue;
	long long enterQueueTime;
	long idQueue;

	static unsigned long long maxTimeInQueue;
};

