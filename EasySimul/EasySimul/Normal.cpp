#include "Normal.h"


Normal::Normal(double m, double s)
{
	normalDist = new std::normal_distribution<double>(m, s);
}


Normal::~Normal()
{
}

unsigned long Normal::getRandom()
{
#ifdef OUTPUT_DEBUG_RAND
	std::cout << "Normal: ";
#endif
	return mod(round((*normalDist)(*mtGenerator)));
}
