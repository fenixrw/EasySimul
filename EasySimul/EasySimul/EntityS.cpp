#include "EntityS.h"


EntityS::EntityS()
{
	totalTimeInService = 0;
	enterServiceTime = -1;
	idService = -1;
}

EntityS::~EntityS()
{
}

unsigned long long EntityS::getTotalTimeInService()
{
	return totalTimeInService;
}


void EntityS::enterService(unsigned long long currentTime, unsigned long serviceID)
{
	idService = serviceID;
	enterServiceTime = currentTime;
}

void EntityS::exitService(unsigned long long currentTime, unsigned long serviceID)
{
	totalTimeInService += currentTime - enterServiceTime;
}
