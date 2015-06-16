#pragma once
#include "Entity.h"
class EntityS : public Entity
{
public:
	EntityS();
	~EntityS();

	void enterService(unsigned long long currentTime, unsigned long serviceID);
	void exitService(unsigned long long currentTime, unsigned long serviceID);

	unsigned long long getTotalTimeInService();

protected:
	unsigned long long totalTimeInService;
	long long enterServiceTime;
	long idService;
};

