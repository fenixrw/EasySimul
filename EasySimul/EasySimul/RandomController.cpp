#include "RandomController.h"


RandomController::RandomController(unsigned int seed)
{
	mtGenerator = new std::mt19937(seed);
	logNormal = new std::lognormal_distribution<double>(2.0179,1.2223);
}


RandomController::~RandomController()
{
}

double RandomController::getRandomLogNormal()
{
	return (*logNormal)(*mtGenerator);
}
