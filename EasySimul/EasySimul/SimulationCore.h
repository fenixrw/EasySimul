#pragma once
#include <vector>
#include "Service.h"
#include "Generator.h"
#include "SimulationNode.h"
class SimulationCore
{
public:
	SimulationCore(unsigned long endTime = 0);
	~SimulationCore();

	void run();

protected:

	unsigned long currentTime;
	unsigned long endAtTime; //0 = disabled

	SimulationNode* getNextNode();
	unsigned long getNextTime();

	std::vector<SimulationNode*> nodes;
	
};

