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
#ifdef OUTPUT_DEBUG_RAND
	std::cout << "Log Normal: ";
#endif
	return mod(round((*logNormal)(*mtGenerator)));
}
