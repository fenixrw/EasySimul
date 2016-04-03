#pragma once
#include <random>
#include <iostream>

class RandomController
{
public:
	RandomController();
	~RandomController();
	virtual unsigned long getRandom() { 
		std::cerr << "Virtual Function <virtual unsigned long getRandom()> Not Implemented" << std::endl;
		std::terminate();
		//perror("Virtual Function Not Implemented"); 
		return rand(); 
	};
	virtual double getRandom_d() { 
		std::cerr << "Virtual Function <virtual double getRandom_d()> Not Implemented" << std::endl;
		std::terminate();
		//perror("Virtual Function Not Implemented"); 
		return rand(); 
	};
	static void init(unsigned int seed = 5489);

protected:

	static unsigned long mod(double value);

	static std::mt19937 *mtGenerator;

};

