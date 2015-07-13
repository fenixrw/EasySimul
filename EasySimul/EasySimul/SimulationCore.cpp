#include "SimulationCore.h"


SimulationCore::SimulationCore()
{
	currentTime = 0;
}


SimulationCore::~SimulationCore()
{
}

unsigned long long SimulationCore::getCurrentTime()
{
	return currentTime;
}

long long SimulationCore::getNextTime()
{
	long long nT = -1;

	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		long long t = nodes[i]->getNextTime(currentTime);

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
	long long nextTime = getNextTime();

	while (nextTime != -1)
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


void SimulationCore::restart()
{
	currentTime = 0;

	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->restart();
	}
}
