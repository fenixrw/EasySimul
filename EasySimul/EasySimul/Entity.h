#pragma once

class Queue;
class Service;

class Entity
{
	friend class Queue;
	friend class Service;

public:
	Entity();
	~Entity();

	virtual void enterSystem(unsigned long long currentTime){};
	virtual void exitSystem(unsigned long long currentTime){};

	virtual void enterQueue(unsigned long long currentTime, unsigned long queueID){};
	virtual void exitQueue(unsigned long long currentTime, unsigned long queueID){};

	virtual void enterService(unsigned long long currentTime, unsigned long serviceID){};
	virtual void exitService(unsigned long long currentTime, unsigned long serviceID){};

	unsigned long getID();
	unsigned long long getTotalTimeInSystem();
	unsigned long long getTotalTimeInService();
	unsigned long long getTotalTimeInQueue();

protected:
	unsigned long id;
	unsigned long long enterSystemTime;
	unsigned long long exitSystemTime;

	unsigned long long totalTimeInService;

	unsigned long long enterQueueTime;
	unsigned long long totalTimeInQueue;


private:
	static unsigned long idController;
};

