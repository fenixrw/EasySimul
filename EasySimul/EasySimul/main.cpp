#include "SimulationCore.h"
#include "LogNormal.h"
#include "Generator.h"
#include "Service.h"
#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	RandomController::init();
		
	LogNormal ln(2.0179,1.2223); //Arrival Distribution

	Queue incomingCalls, exitQueue;

	Generator inGen(&ln);
	inGen.setOutputQueue(&incomingCalls);
	inGen.setTimeLimit(60);
		
	Service attendent(&ln);
	attendent.setInputQueue(&incomingCalls);
	attendent.setOutputQueue(&exitQueue);

	Service attendent2(&ln);
	attendent2.setInputQueue(&incomingCalls);
	attendent2.setOutputQueue(&exitQueue);

	SimulationCore core;
	core.add(&inGen);
	core.add(&attendent);
	core.run();

	std::cout << exitQueue.getSize() << endl;

	system("pause");

	//RandomController rc;

	//ofstream file;

	//file.open("output.txt");

	//for (int i = 0; i < 130; i++)
	//	file << rc.getRandomLogNormal() << endl;

	//file.close();

	return 0;
}