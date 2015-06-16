#pragma once

class Entity
{
public:
	Entity();
	~Entity();

	void enterSystem(unsigned long long currentTime);
	void exitSystem(unsigned long long currentTime);
	virtual void enterQueue(unsigned long long currentTime, unsigned long queueID){};
	virtual void exitQueue(unsigned long long currentTime, unsigned long queueID){};

	unsigned long long getTotalTimeInSystem();	
	unsigned long getID();

protected:
	unsigned long id;
	unsigned long long enterSystemTime;
	unsigned long long exitSystemTime;

private:
	static unsigned long idController;
};

