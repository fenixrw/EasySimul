#pragma once

#include <random>

class RandomController
{
public:
	RandomController(unsigned int seed = 5489);
	~RandomController();
	double getRandomLogNormal();

protected:

	std::mt19937 *mtGenerator;
	std::lognormal_distribution<double> *logNormal;

};

