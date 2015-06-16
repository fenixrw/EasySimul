#include "Entity.h"

unsigned long Entity::idController = 0;

Entity::Entity()
{
	id = idController;
	idController++;
	enterSystemTime = 0;
	exitSystemTime = 0;
}

Entity::~Entity()
{
}


unsigned long long Entity::getTotalTimeInSystem()
{
	return exitSystemTime - enterSystemTime;
}

unsigned long Entity::getID()
{
	return id;
}

void Entity::enterSystem(unsigned long long currentTime)
{
	enterSystemTime = currentTime;
}

void Entity::exitSystem(unsigned long long currentTime)
{
	exitSystemTime = currentTime;
}