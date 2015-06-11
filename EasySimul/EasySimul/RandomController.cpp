#include "RandomController.h"

std::mt19937 *RandomController::mtGenerator = NULL;

RandomController::RandomController()
{
	//std::lognormal_distribution<double> *logNormal;
	//logNormal = new std::lognormal_distribution<double>(2.0179,1.2223);
}

void RandomController::init(unsigned int seed)
{
	mtGenerator = new std::mt19937(seed);
}

RandomController::~RandomController()
{
}

//double RandomController::getRandomLogNormal()
//{
//	return (*logNormal)(*mtGenerator);
//}
