#pragma once
class SimulationNode
{
public:
	SimulationNode();
	~SimulationNode();

	virtual unsigned long getNexTime();

	virtual void update(unsigned long currentTime);
	
protected:

	unsigned long nextTime;
};

