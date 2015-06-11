#include "SimulationCore.h"


SimulationCore::SimulationCore()
{
	currentTime = 0;
}


SimulationCore::~SimulationCore()
{
}

SimulationNode* SimulationCore::getNextNode()
{
	SimulationNode* node = NULL;

	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		long long t = nodes[i]->getNexTime(currentTime);
		if (t >= 0)
		{
			if (node != NULL) //if there is a selected node
			{
				//Compare the times
				if (t < node->getNexTime(currentTime))
				{
					node = nodes[i];
				}
			}
			else //if there is none selected node, select the first one
			{
				node = nodes[i];
			}
		}
	}

	return node;
}

long long SimulationCore::getNextTime()
{
	long long nT = -1;

	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		long long t = nodes[i]->getNexTime(currentTime);
		if (t >= 0)
		{
			if (nT >= 0) //if there is a selected node
			{
				//Compare the times
				if (t < nT)
				{
					nT = t;
				}
			}
			else //if there is none selected node, select the first one
			{
				nT = t;
			}
		}
	}

	return nT;
}

void SimulationCore::add(SimulationNode* n)
{
	nodes.push_back(n);
}

void SimulationCore::run()
{
	//First Client
	//Add time

	//while currentTime < endAtTime
		//Check list of events in the current time
			//run events if time is right
		//check next time
		//set next current time

	long long nextTime = getNextTime();

	while (nextTime!=-1)
	{
		currentTime = nextTime;

		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->update(currentTime);
		}

		nextTime = getNextTime();
	}
	
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->end(currentTime);
	}
}