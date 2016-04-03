#pragma once
#include "RandomController.h"
#include "Queue.h"

class SimulationNode
{
public:
	SimulationNode();
	~SimulationNode();

	virtual long long getNextTime(unsigned long long currentTime){ return -1; };
	virtual void update(unsigned long long currentTime){};
	virtual void end(unsigned long long currentTime){};
	virtual void restart(){};
};

