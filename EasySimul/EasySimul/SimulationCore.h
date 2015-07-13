#pragma once
#include <vector>
#include "Service.h"
#include "Generator.h"
#include "SimulationNode.h"
class SimulationCore
{
public:
	SimulationCore();
	~SimulationCore();

	void add(SimulationNode* n);
	void run();
	void restart();


	unsigned long long getCurrentTime();

protected:

	unsigned long long currentTime;

	long long getNextTime();

	std::vector<SimulationNode*> nodes;
	
};

