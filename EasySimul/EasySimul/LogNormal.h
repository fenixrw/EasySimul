#pragma once
#include "RandomController.h"

class LogNormal :
	public RandomController
{
public:
	LogNormal(double m = 2.0179, double s = 1.2223);
	~LogNormal();

	virtual unsigned long getRandom();

protected:

	std::lognormal_distribution<double> *logNormal;
};

