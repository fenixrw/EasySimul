#include "UniformNormal3.h"


UniformNormal3::UniformNormal3(unsigned int probability[3], double m[3], double s[3])
{
	limit[0] = probability[0];
	limit[1] = limit[0] + probability[1];
	limit[2] = limit[1] + probability[2];
	uniformDist = new std::uniform_int_distribution<unsigned int>(0,limit[2]);
	for (int i = 0; i < 3; i++)
		normalDist[i] = new std::normal_distribution<double>(m[i], s[i]);
}


UniformNormal3::~UniformNormal3()
{
}

unsigned long UniformNormal3::getRandom()
{
	unsigned int num = (*uniformDist)(*mtGenerator);

	for (int i = 0; i < 3; i++)
	{
		if (num <= limit[i])
		{
#ifdef OUTPUT_DEBUG_RAND
			std::cout << "U3Normal: ";
#endif
			return mod(round((*normalDist[i])(*mtGenerator)));
		}
	}
#ifdef OUTPUT_DEBUG_RAND
	std::cout << "[X]U3Normal: ";
#endif
	return mod(round((*normalDist[2])(*mtGenerator)));
}
