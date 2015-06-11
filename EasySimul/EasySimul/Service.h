#pragma once
#include "SimulationNode.h"
#include "RandomController.h"
#include "Entity.h"
#include "Queue.h"

class Service : public SimulationNode
{
public:
	Service();
	~Service();

protected:

	int finishServiceTime; /*-1 = idle (None Entity)*/
	Queue* entryQueue;
	Queue* exitQueue;

};

