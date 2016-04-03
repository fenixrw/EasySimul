#include "ContextEntity.h"

unsigned long long ContextEntity::finishTime = 60 * 60 * 24;
unsigned long long ContextEntity::updateScheduleTime = 30;
unsigned long long ContextEntity::idCounter = 0;

ContextEntity::ContextEntity(RandomController *_directionRand)
{
	id = idCounter;
	idCounter++;

	directionRand = _directionRand;

	speed = 1.f/3600.f; // 1km/h

	nextActionTime = 0;
	nextUpdateTime = updateScheduleTime;

	changeStateCounter = 5;

	currentContext.latitude = 0;
	currentContext.longitude = 0;
	currentContext.state = STATE_READY;
	currentContext.timestamp = 0;

	dir_lat = 0;
	dir_lon = 0;
}

ContextEntity::~ContextEntity()
{
}

long long ContextEntity::getNextTime(unsigned long long currentTime)
{
	unsigned long long nextTime = ((nextUpdateTime < nextActionTime) ? nextUpdateTime : nextActionTime);

	if (currentTime >= finishTime || nextTime > finishTime)
	{
		return -1;
	}

	return nextTime;
}

void ContextEntity::update(unsigned long long currentTime)
{
	if (nextActionTime == currentTime)
	{
		switch (currentContext.state)
		{
		case STATE_READY:
			//chenge direction
			nextActionTime += 60;
			break;
		case STATE_MISSION:
			//change direction
			nextActionTime += 60*2;
			break;
		case STATE_INJURIED:
			// ?? change direction ??
			nextActionTime += 60*3;
			break;
		default:
			break;
		}

		//dir_lat = (float)(directionRand->getRandom())/3000.f; //TODO: add getRandom with type double return value
		//dir_lon = (float)(directionRand->getRandom())/3000.f;

		dir_lat = directionRand->getRandom_d();
		dir_lon = directionRand->getRandom_d();

		changeStateCounter--;

		if (changeStateCounter == 0)
		{
			unsigned int chance = rand() % 100;

			if (chance < 60)
			{
				currentContext.state = STATE_READY;
				changeStateCounter = 10;
			}
			else if (chance < 90)
			{
				currentContext.state = STATE_MISSION;
				changeStateCounter = 30;
			}
			else
			{
				currentContext.state = STATE_INJURIED;
				changeStateCounter = 60;
			}
		}

		//Check State

			//Change Direction?

			//Change State?
			
			//select next action time (time of current action)
	}

	if (nextUpdateTime == currentTime)
	{
		//Check State
			//update DynamicContext variables if needed

		currentContext.latitude += dir_lat*speed;
		currentContext.longitude += dir_lon*speed;
		currentContext.timestamp = currentTime;

		contextOverTime.push_back(currentContext); //save current context
		nextUpdateTime += updateScheduleTime; //set next update time
	}
}

void ContextEntity::end(unsigned long long currentTime)
{
	cout << "Entity " << id << " >> Number of Logs = " << contextOverTime.size() << endl;

	string states[3] = {"READY","MISSION","INJURIED"};

	if (/*id == 0*/true)
	{
	/*	for (int i = 0; i < 20; i++)
		{
			cout << "    - Time:" << contextOverTime[i].timestamp << ", State: " << states[contextOverTime[i].state] << ", Position: [" << contextOverTime[i].latitude << ", " << contextOverTime[i].longitude << "]" << endl;
		}*/

		unsigned short lastState = 3;
		for (int i = 0; i < contextOverTime.size(); i++)
		{
			if (lastState != contextOverTime[i].state)
			{
				cout << "    - Time:" << contextOverTime[i].timestamp << ", State: " << states[contextOverTime[i].state] << ", Position: [" << contextOverTime[i].latitude << ", " << contextOverTime[i].longitude << "]" << endl;
				lastState = contextOverTime[i].state;
			}
		}
	}
}

void ContextEntity::restart()
{
}
