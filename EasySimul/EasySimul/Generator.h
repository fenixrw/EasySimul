#pragma once
#include "RandomController.h"
#include "SimulationNode.h"

class Generator : public SimulationNode
{
public:
	Generator(RandomController *r);
	~Generator();

protected:

	RandomController *randControl;
};

