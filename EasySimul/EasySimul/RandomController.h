#pragma once
#include <random>

class RandomController
{
public:
	RandomController();
	~RandomController();
	virtual unsigned long getRandom(){ return -1; };
	static void init(unsigned int seed = 5489);

protected:

	static std::mt19937 *mtGenerator;

};

