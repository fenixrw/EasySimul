#include "SimulationCore.h"


SimulationCore::SimulationCore()
{
	currentTime = 0;
	simulationNumber = 0;
	timesToRepeat = 1;
}


SimulationCore::~SimulationCore()
{
}

void SimulationCore::setRepeat(unsigned long numberOfSimulations)
{
	timesToRepeat = numberOfSimulations;
}

unsigned long long SimulationCore::getCurrentTime()
{
	return currentTime;
}

SimulationNode* SimulationCore::getNextNode()
{
	SimulationNode* node = NULL;

	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		long long t = nodes[i]->getNextTime(currentTime);
		if (t >= 0)
		{
			if (node != NULL) //if there is a selected node
			{
				//Compare the times
				if (t < node->getNextTime(currentTime))
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

	//std::cout << "Time Select <" << currentTime << "> = {";
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		long long t = nodes[i]->getNextTime(currentTime);
		//std::cout << t << ", ";

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

	//std::cout << " <" << nT << ">}" << std::endl;

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
	while (true)
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

		simulationNumber++;

		if (simulationNumber < timesToRepeat)
		{
			currentTime = 0;
			for (unsigned int i = 0; i < nodes.size(); i++)
			{
				nodes[i]->restart();
			}
		}
		else
		{
			break;
		}
	}
}