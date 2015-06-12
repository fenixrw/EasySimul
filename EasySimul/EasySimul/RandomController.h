#pragma once
#include <random>
#include <iostream>

class RandomController
{
public:
	RandomController();
	~RandomController();
	virtual unsigned long getRandom(){ return -1; };
	static void init(unsigned int seed = 5489);

protected:

	static unsigned long mod(double value);

	static std::mt19937 *mtGenerator;

};

