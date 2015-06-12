#pragma once
#include "RandomController.h"
class UniformNormal3 :
	public RandomController
{
public:
	UniformNormal3(unsigned int probability[3]/*0-100*/, double m[3], double s[3]);
	~UniformNormal3();

	virtual unsigned long getRandom();

protected:

	unsigned int limit[3];
	std::uniform_int_distribution<unsigned int> *uniformDist;
	std::normal_distribution<double> *normalDist[3];
};

