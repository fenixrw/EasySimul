#pragma once
#include "Entity.h"
#include <queue>

class Queue
{
public:
	Queue();
	~Queue();

	int getSize();
	bool getNext(Entity &e);

protected:

	std::queue<Entity> entityQueue;

};

