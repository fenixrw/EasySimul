#include "SimulationCore.h"
#include "LogNormal.h"
#include "Normal.h"
#include "UniformNormal3.h"
#include "Generator.h"
#include "Service.h"
#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	unsigned long simulationTime = 10*60;
	RandomController::init(5489*7^3);
		
	LogNormal lnDist(2.0179,1.2223); //Arrival Distribution
	Normal nDist(15.714, 11.604);
	unsigned int prob[3] = { 2, 38, 60 };
	double m3[3] = { 1, 8, 2 };
	double s3[3] = { 0.005, 1, 0.005 };
	UniformNormal3 un3Dist(prob, m3, s3);

	Queue incomingCalls, exitQueue, exitQueue2, emptyQueue;

	Generator inGen(&lnDist);
	inGen.setOutputQueue(&incomingCalls);
	inGen.setTimeLimit(simulationTime);
		
	Service attendent1(&nDist);
	attendent1.setInputQueue(&incomingCalls);
	attendent1.setOutputQueue(&exitQueue);
	attendent1.setTurn(-1, 6 * 60);

	Service attendent1b(&nDist);
	attendent1b.setInputQueue(&incomingCalls);
	attendent1b.setOutputQueue(&exitQueue);
	attendent1b.setTurn(-1, 6 * 60);
	
	Service attendent2(&nDist);
	attendent2.setInputQueue(&incomingCalls);
	attendent2.setOutputQueue(&exitQueue);
	attendent2.setTurn(4 * 60, -1);
	//attendent2.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);

	Service attendent2b(&nDist);
	attendent2b.setInputQueue(&incomingCalls);
	attendent2b.setOutputQueue(&exitQueue);
	attendent2b.setTurn(4 * 60, -1);
	//attendent2b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);

	Service attendent3(&nDist);
	attendent3.setInputQueue(&emptyQueue);
	attendent3.setOutputQueue(&exitQueue);
	attendent3.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent3.setTurn(-1, 6 * 60);

	Service attendent3b(&nDist);
	attendent3b.setInputQueue(&emptyQueue);
	attendent3b.setOutputQueue(&exitQueue);
	attendent3b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent3b.setTurn(-1, 6 * 60);

	Service attendent4(&nDist);
	attendent4.setInputQueue(&emptyQueue);
	attendent4.setOutputQueue(&exitQueue);
	attendent4.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent4.setTurn(4 * 60, -1);

	Service attendent4b(&nDist);
	attendent4b.setInputQueue(&emptyQueue);
	attendent4b.setOutputQueue(&exitQueue);
	attendent4b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent4b.setTurn(4 * 60, -1);

	Service attendent4c(&nDist);
	attendent4c.setInputQueue(&emptyQueue);
	attendent4c.setOutputQueue(&exitQueue);
	attendent4c.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent4c.setTurn(4 * 60, -1);

	SimulationCore core;
	core.add(&inGen);
	core.add(&attendent1);
	core.add(&attendent2);
	core.add(&attendent3);
	core.add(&attendent4);
	core.add(&attendent1b);
	core.add(&attendent2b);
	core.add(&attendent3b);
	core.add(&attendent4b);
	core.add(&attendent4c);
	core.run();

	//cout << endl << endl;
	cout << "Clients Entered The Sistem: " << inGen.entityCounter << endl;
	cout << "Clients Exited The Sistem: " << exitQueue.getSize() << endl;
	cout << "Total Idle Time For Attendent 1: " << attendent1.totalIdleTime << endl;
	cout << "Total Idle Time For Attendent 1b: " << attendent1b.totalIdleTime << endl;
	cout << "Total Idle Time For Attendent 2: " << attendent2.totalIdleTime << endl;
	cout << "Total Idle Time For Attendent 2b: " << attendent2b.totalIdleTime << endl;
	cout << "Total Secondary Service Done: " << exitQueue2.getSize() << endl;
	cout << "End Time: " << core.getCurrentTime() << endl;

	system("pause");

	//RandomController rc;

	//ofstream file;

	//file.open("output.txt");

	//for (int i = 0; i < 130; i++)
	//	file << rc.getRandomLogNormal() << endl;

	//file.close();

	return 0;
}