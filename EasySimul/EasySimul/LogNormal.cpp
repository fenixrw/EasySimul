#include "LogNormal.h"


LogNormal::LogNormal(double m, double s)
{
	logNormal = new std::lognormal_distribution<double>(m,s);
}


LogNormal::~LogNormal()
{
}

unsigned long LogNormal::getRandom()
{
	return (*logNormal)(*mtGenerator);
}
