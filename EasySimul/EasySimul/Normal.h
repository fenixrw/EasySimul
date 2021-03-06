#pragma once
#include "RandomController.h"
class Normal :
	public RandomController
{
public:
	Normal(double m = 15.714, double s = 11.604);
	~Normal();
	virtual unsigned long getRandom();

protected:

	std::normal_distribution<double> *normalDist;
};

