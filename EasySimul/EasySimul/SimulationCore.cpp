#include "SimulationCore.h"


SimulationCore::SimulationCore(unsigned long endTime)
{
	endAtTime = endTime;
	currentTime = 0;
}


SimulationCore::~SimulationCore()
{
}

SimulationNode* SimulationCore::getNextNode()
{
	SimulationNode* node = NULL;

	for (int i = 0; i < nodes.size(); i++)
	{
		int t = nodes[i]->getNexTime();
		if (t >= 0)
		{
			if (node != NULL) //if there is a selected node
			{
				//Compare the times
				if (t < node->getNexTime())
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

unsigned long SimulationCore::getNextTime()
{
	unsigned long nT = -1;

	for (int i = 0; i < nodes.size(); i++)
	{
		int t = nodes[i]->getNexTime();
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

void SimulationCore::run()
{
	//First Client
	//Add time

	//while currentTime < endAtTime
		//Check list of events in the current time
			//run events if time is right
		//check next time
		//set next current time

	int nextTime = getNextTime();

	while (nextTime!=-1)
	{
		currentTime = nextTime;

		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->update(currentTime);
		}

		nextTime = getNextTime();
	}
}