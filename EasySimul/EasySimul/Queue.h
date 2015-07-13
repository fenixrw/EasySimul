#pragma once
#include "Entity.h"
#include <queue>
#include <map>
#include <vector>

typedef enum _QueueType
{
	NormalQueue=0, EntryQueue, ExitQueue
}QUEUE_TYPE;

class Queue
{
public:
	Queue(QUEUE_TYPE t = NormalQueue);
	~Queue();

	int getSize();
	Entity * getNext(unsigned long long currentTime);
	bool add(Entity *e, unsigned long long currentTime);

	std::map<unsigned long, unsigned long long> getTimePerSize();
	unsigned long getMaxSize();
	unsigned long getID();
	std::vector<Entity*> emptyToVector();
	void end(unsigned long long currentTime);
	void restart();
	
protected:
	unsigned long id;
	std::queue<Entity*> entityQueue;
	QUEUE_TYPE type;
	unsigned long maxSize;
	unsigned long long lastOperationTime;
	std::map<unsigned long, unsigned long long> timePerSize;  //[size] --> time

private:
	static unsigned long idController;

};

