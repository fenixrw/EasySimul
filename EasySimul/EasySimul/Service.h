#pragma once
#include "Entity.h"
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

	void setTurn(long long startT, long long endT);

	unsigned long getID();

	unsigned long long getIdleTime();
	unsigned long long getSimulationTime();
	unsigned long getEntityCounter();
	
protected:
	
	unsigned long id;
	void getNextEntity(unsigned long long currentTime);
	unsigned long simulation; /*Simulation Day/Number */
	unsigned long timeLimit;
	long long finishServiceTime; /*-1 = idle (None Entity)*/
	long long lastServiceTime; /*starts at 0*/
	long long startTurn;
	long long endTurn;
	Entity* entityInService;	

	unsigned long entityCounter;
	unsigned long long idleTime;
	unsigned long long simulationTime;

	Queue* entryQueue;
	Queue* outputQueue;
	
private:
	static unsigned long idController;
};