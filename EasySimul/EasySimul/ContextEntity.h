#pragma once
#include "SimulationNode.h"
#include <vector>
#include <string>
using namespace std;

#define STATE_READY 0
#define STATE_MISSION 1
#define STATE_INJURIED 2


class ContextEntity :
	public SimulationNode
{
public:
	ContextEntity(RandomController *_directionRand);
	~ContextEntity();

	virtual long long getNextTime(unsigned long long currentTime);
	virtual void update(unsigned long long currentTime);
	virtual void end(unsigned long long currentTime);
	virtual void restart();

	static unsigned long long finishTime;
	static unsigned long long updateScheduleTime;

protected:
	static unsigned long long idCounter;

	unsigned short changeStateCounter;

	double speed;

	unsigned long long id;

	RandomController *directionRand, *actionRand, *recoveryRand, *timeRand;

	unsigned long long nextActionTime, nextUpdateTime;

	double dir_lat, dir_lon;
	
	struct DynamicContext
	{
		unsigned long long timestamp;
		double longitude, latitude;
		unsigned short state;
	};

	DynamicContext currentContext;
	
	vector<DynamicContext> contextOverTime;
};

