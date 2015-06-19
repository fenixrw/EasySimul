#pragma once
#include "OutgoingEntity.h"
#include "IncomingEntity.h"
#include "SimulationNode.h"

class Service : public SimulationNode
{
public:
	Service(RandomController *r);
	~Service();

	virtual long long getNextTime(unsigned long long currentTime);
	virtual void update(unsigned long long currentTime);
	virtual void end(unsigned long long currentTime);
	virtual void restart();


	void setOutputQueue(Queue *out);
	void setInputQueue(Queue *in);
	void setSecondaryService(RandomController *r, Queue *out, unsigned long tLimit);
	void activeFreeIncomingServiceRule();

	void setTurn(long long startT, long long endT);


	unsigned long getEntityIncomingCounter();
	unsigned long getEntityOutgoingCounter();
	unsigned long long getTotalIdleTime();
	unsigned long getID();

	unsigned long long getTotalSimulationTime();

	static unsigned int minFreeServices;

protected:

	static unsigned int freeServices;
	bool freeIncomingServiceRule;

	unsigned long id;
	void getNextEntity(unsigned long long currentTime);
	unsigned long simulation; /*Simulation Day/Number */
	unsigned long timeLimit;
	long long finishServiceTime; /*-1 = idle (None Entity)*/
	long long lastServiceTime; /*starts at 0*/
	long long startTurn;
	long long endTurn;
	EntityS* entityInService;	

	unsigned long entityIncomingCounter;
	unsigned long entityOutgoingCounter;
	unsigned long long totalIdleTime;
	unsigned long long totalSimulationTime;

	Queue* entryQueue;
	Queue* outputQueue;
	Queue * primaryOutputQueue;
	Queue * secondaryOutputQueue;
	RandomController *secondaryRandControl;
	
private:
	static unsigned long idController;
	bool freeIncomingServiceRuleStarted;
};

