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
	//unsigned long long a = 240;
	//long long b = a;
	//cout << a << "=" << b << endl;
	//system("pause");
	//bool addAttendent2c = true;
	//bool useDoubleAssignmentIncoming = true;
	//bool useDoubleAssignmentOutgoing = true;

	bool addAttendent2c = false;
	bool useDoubleAssignmentIncoming = false;
	bool useDoubleAssignmentOutgoing = false;

	unsigned long simulationTime = 10 * 60;
	unsigned long simulationRepeats = 200;
	RandomController::init(5489);
		
	LogNormal lnDist(2.0179,1.2223); //Arrival Distribution
	Normal nDist(15.714, 11.604);
	unsigned int prob[3] = { 3, 39, 58 };
	double m3[3] = { 2.5, 13, 4 };//{ 1, 8, 2 };//
	double s3[3] = { 0.005, 1, 0.005 };
	UniformNormal3 un3Dist(prob, m3, s3);

	Queue incomingCalls, exitQueue, exitQueue2, emptyQueue;

	Generator inGen(&lnDist);
	inGen.setOutputQueue(&incomingCalls);
	inGen.setTimeLimit(simulationTime);

	//----------------------------- INCOMING CALLS -----------------------------

	Service attendent1(&nDist);
	attendent1.setInputQueue(&incomingCalls);
	attendent1.setOutputQueue(&exitQueue);
	attendent1.setTurn(-1, 6 * 60);
	if (useDoubleAssignmentIncoming)
		attendent1.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);

	Service attendent1b(&nDist);
	attendent1b.setInputQueue(&incomingCalls);
	attendent1b.setOutputQueue(&exitQueue);
	attendent1b.setTurn(-1, 6 * 60);
	if (useDoubleAssignmentIncoming)
		attendent1b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	
	Service attendent2(&nDist);
	attendent2.setInputQueue(&incomingCalls);
	attendent2.setOutputQueue(&exitQueue);
	attendent2.setTurn(4 * 60, -1);
	if (useDoubleAssignmentIncoming)
		attendent2.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);

	Service attendent2b(&nDist);
	attendent2b.setInputQueue(&incomingCalls);
	attendent2b.setOutputQueue(&exitQueue);
	attendent2b.setTurn(4 * 60, -1);
	if (useDoubleAssignmentIncoming)
		attendent2b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	
	Service attendent2c(&nDist);
	attendent2c.setInputQueue(&incomingCalls);
	attendent2c.setOutputQueue(&exitQueue);
	attendent2c.setTurn(4 * 60, -1);
	if (useDoubleAssignmentIncoming)
		attendent2c.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);

	//----------------------------- OUTGOING CALLS -----------------------------

	Queue *q = &emptyQueue;

	if (useDoubleAssignmentOutgoing)
		q = &incomingCalls;

	Service attendent3(&nDist);
	attendent3.setInputQueue(q);
	attendent3.setOutputQueue(&exitQueue);
	attendent3.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent3.setTurn(-1, 6 * 60);

	Service attendent3b(&nDist);
	attendent3b.setInputQueue(q);
	attendent3b.setOutputQueue(&exitQueue);
	attendent3b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent3b.setTurn(-1, 6 * 60);

	Service attendent4(&nDist);
	attendent4.setInputQueue(q);
	attendent4.setOutputQueue(&exitQueue);
	attendent4.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent4.setTurn(4 * 60, -1);

	Service attendent4b(&nDist);
	attendent4b.setInputQueue(q);
	attendent4b.setOutputQueue(&exitQueue);
	attendent4b.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent4b.setTurn(4 * 60, -1);

	Service attendent4c(&nDist);
	attendent4c.setInputQueue(q);
	attendent4c.setOutputQueue(&exitQueue);
	attendent4c.setSecondaryService(&un3Dist, &exitQueue2, simulationTime);
	attendent4c.setTurn(4 * 60, -1);

	//----------------------------- CORE -----------------------------

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
	if (addAttendent2c)
		core.add(&attendent2c);
	core.add(&attendent4c);
	core.setRepeat(simulationRepeats);
	core.run();

	cout << endl << "----------------------- Total --------------------------" << endl << endl;
	cout << "	Clients Entered The Sistem	: " << inGen.entityCounter << endl;
	cout << "	Clients Exited The Sistem	: " << exitQueue.getSize() << endl;
	cout << "	Total Idle Time For Attendent 1	: " << attendent1.totalIdleTime << endl;
	cout << "	Total Idle Time For Attendent 1b: " << attendent1b.totalIdleTime << endl;
	cout << "	Total Idle Time For Attendent 2	: " << attendent2.totalIdleTime << endl;
	cout << "	Total Idle Time For Attendent 2b: " << attendent2b.totalIdleTime << endl;
	cout << "	Total Idle Time For Attendent 2c: " << attendent2c.totalIdleTime << endl;
	cout << "	Total Secondary Service Done	: " << exitQueue2.getSize() << endl;
	cout << "	Secondary Service Per Attendent	: " << exitQueue2.getSize() / 5 << endl;
	if (simulationRepeats == 1)
	{
		cout << "End Time: " << core.getCurrentTime() << endl;
	}
	else
	{
		cout << endl << "----------------------- Median --------------------------" << endl << endl;
		cout << "	Clients Entered The Sistem	: " << inGen.entityCounter/simulationRepeats << endl;
		cout << "	Clients Exited The Sistem	: " << exitQueue.getSize() / simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 1	: " << attendent1.totalIdleTime / simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 1b: " << attendent1b.totalIdleTime / simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 2	: " << attendent2.totalIdleTime / simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 2b: " << attendent2b.totalIdleTime / simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 2c: " << attendent2c.totalIdleTime / simulationRepeats << endl;
		cout << "	Total Secondary Service Done	: " << exitQueue2.getSize() / simulationRepeats << endl;
		cout << "	Secondary Service Per Attendent	: " << (exitQueue2.getSize() / 5) / simulationRepeats << endl;
	}

	cout << endl;
	system("pause");

	//RandomController rc;

	//ofstream file;

	//file.open("output.txt");

	//for (int i = 0; i < 130; i++)
	//	file << rc.getRandomLogNormal() << endl;

	//file.close();

	return 0;
}