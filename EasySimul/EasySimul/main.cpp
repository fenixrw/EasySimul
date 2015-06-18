#include "SimulationCore.h"
#include "LogNormal.h"
#include "Normal.h"
#include "UniformNormal3.h"
#include "Generator.h"
#include "Service.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <tclap/CmdLine.h>

using namespace std;

typedef struct _simulCongif
{
	bool addAttendent;
	bool useDoubleAssignmentIncoming;
	bool useDoubleAssignmentOutgoing;

	unsigned long simulationTime;
	unsigned long simulationRepeats;

	unsigned int seed;

}SimulConfig;

using namespace TCLAP;

void getConfigInfo(SimulConfig &config, int argc, char** argv)
{
	try {

		CmdLine cmd("Easy Simul <Examples> - Call Center Simulation", ' ', "1.0");

		ValueArg<unsigned int> seedArg("s", "seed", "Set simulation seed.", false, 5489, "uint");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(seedArg);

		SwitchArg newAttendantSwitch("a", "add", "Add 1 attendant.", false);
		cmd.add(newAttendantSwitch);

		SwitchArg outgoingSwitch("o", "outgoing", "Set outgoing service for all attendants.", false);
		cmd.add(outgoingSwitch);

		SwitchArg incomingSwitch("i", "incomig", "Set incoming service for all attendants.", false);
		cmd.add(incomingSwitch);

		ValueArg<unsigned long> repeatArg("r", "repeat", "Set number of times to repeat the simulation.", false, 1, "ulong");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(repeatArg);

		ValueArg<unsigned long> timeArg("t", "time", "Set simulation time in minutes.", false, 600, "");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(timeArg);

		cmd.parse(argc, argv);

		config.addAttendent = newAttendantSwitch.getValue();
		config.seed = seedArg.getValue();
		config.simulationRepeats = repeatArg.getValue();
		config.simulationTime = timeArg.getValue();
		config.useDoubleAssignmentIncoming = incomingSwitch.getValue();
		config.useDoubleAssignmentOutgoing = outgoingSwitch.getValue();

	}
	catch (ArgException &e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}
}


int main(int argc, char** argv)
{
	//unsigned long long a = 240;
	//long long b = a;
	//cout << a << "=" << b << endl;
	//system("pause");
	//bool addAttendent2c = true;
	//bool useDoubleAssignmentIncoming = true;
	//bool useDoubleAssignmentOutgoing = true;

	SimulConfig config;
	
	getConfigInfo(config, argc, argv);

	/*bool addAttendent2c = false;
	bool useDoubleAssignmentIncoming = false;
	bool useDoubleAssignmentOutgoing = false;

	unsigned long simulationTime = 10 * 60;
	unsigned long simulationRepeats = 1;*/

	RandomController::init(config.seed);//5489
		
	LogNormal lnDist(2.0179,1.2223); //Arrival Distribution
	Normal nDist(15.714, 11.604);
	unsigned int prob[3] = { 3, 39, 58 };
	double m3[3] = { 2.5, 13, 4 };//{ 1, 8, 2 };//
	double s3[3] = { 0.005, 1, 0.005 };
	UniformNormal3 un3Dist(prob, m3, s3);

	Queue incomingCalls(EntryQueue), exitQueue(ExitQueue), exitQueue2(ExitQueue), emptyQueue;

	Generator inGen(&lnDist);
	inGen.setOutputQueue(&incomingCalls);
	inGen.setTimeLimit(config.simulationTime);

	//----------------------------- INCOMING CALLS -----------------------------

	Service attendent1(&nDist);
	attendent1.setInputQueue(&incomingCalls);
	attendent1.setOutputQueue(&exitQueue);
	attendent1.setTurn(-1, 6 * 60);
	if (config.useDoubleAssignmentOutgoing)
		attendent1.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);

	Service attendent1b(&nDist);
	attendent1b.setInputQueue(&incomingCalls);
	attendent1b.setOutputQueue(&exitQueue);
	attendent1b.setTurn(-1, 6 * 60);
	if (config.useDoubleAssignmentOutgoing)
		attendent1b.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	
	Service attendent2(&nDist);
	attendent2.setInputQueue(&incomingCalls);
	attendent2.setOutputQueue(&exitQueue);
	attendent2.setTurn(4 * 60, -1);
	if (config.useDoubleAssignmentOutgoing)
		attendent2.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);

	Service attendent2b(&nDist);
	attendent2b.setInputQueue(&incomingCalls);
	attendent2b.setOutputQueue(&exitQueue);
	attendent2b.setTurn(4 * 60, -1);
	if (config.useDoubleAssignmentOutgoing)
		attendent2b.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	
	Service attendent2c(&nDist);
	attendent2c.setInputQueue(&incomingCalls);
	attendent2c.setOutputQueue(&exitQueue);
	attendent2c.setTurn(4 * 60, -1);
	if (config.useDoubleAssignmentOutgoing)
		attendent2c.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);

	//----------------------------- OUTGOING CALLS -----------------------------

	Queue *q = &emptyQueue;

	if (config.useDoubleAssignmentIncoming)
		q = &incomingCalls;

	Service attendent3(&nDist);
	attendent3.setInputQueue(q);
	attendent3.setOutputQueue(&exitQueue);
	attendent3.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendent3.setTurn(-1, 6 * 60);

	Service attendent3b(&nDist);
	attendent3b.setInputQueue(q);
	attendent3b.setOutputQueue(&exitQueue);
	attendent3b.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendent3b.setTurn(-1, 6 * 60);

	Service attendent4(&nDist);
	attendent4.setInputQueue(q);
	attendent4.setOutputQueue(&exitQueue);
	attendent4.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendent4.setTurn(4 * 60, -1);

	Service attendent4b(&nDist);
	attendent4b.setInputQueue(q);
	attendent4b.setOutputQueue(&exitQueue);
	attendent4b.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendent4b.setTurn(4 * 60, -1);

	Service attendent4c(&nDist);
	attendent4c.setInputQueue(q);
	attendent4c.setOutputQueue(&exitQueue);
	attendent4c.setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
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
	if (config.addAttendent)
		core.add(&attendent2c);
	core.add(&attendent4c);
	core.setRepeat(config.simulationRepeats);
	core.run();

	cout << endl << "------------------- Total Output <of " << config.simulationRepeats << " simulation(s)> -------------------" << endl << endl;
	cout << "	Total Incoming Clients		: " << inGen.entityCounter << endl;
	cout << "	Total Idle Time For Attendent 1	: " << attendent1.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 1b: " << attendent1b.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 2	: " << attendent2.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 2b: " << attendent2b.getTotalIdleTime() << endl;
	if (config.addAttendent)
		cout << "	Total Idle Time For Attendent 2c: " << attendent2c.getTotalIdleTime() << endl;

	cout << "	Total Idle Time For Attendent 3	: " << attendent3.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 3b: " << attendent3b.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 4	: " << attendent4.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 4b: " << attendent4b.getTotalIdleTime() << endl;
	cout << "	Total Idle Time For Attendent 4c: " << attendent4b.getTotalIdleTime() << endl;

	cout << "	Total Secondary Service Done	: " << exitQueue2.getSize() << endl;
	cout << "	Secondary Service Per Attendent	: " << exitQueue2.getSize() / 5 << endl;
	cout << "	Incoming Call Max Time In Queue	: " << IncomingEntity::getMaxTimeInQueue() << endl;
	if (config.simulationRepeats == 1)
	{
		cout << "End Time: " << core.getCurrentTime() << endl;
	}
	else
	{
		cout << endl << "------------------- Median Output <of " << config.simulationRepeats << " simulation(s)> -------------------" << endl << endl;
		cout << "	Incoming Clients / Simulation	: " << inGen.entityCounter / config.simulationRepeats << endl;
		cout << "	Clients Exited The Sistem	: " << exitQueue.getSize() / config.simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 1	: " << attendent1.getTotalIdleTime() / config.simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 1b: " << attendent1b.getTotalIdleTime() / config.simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 2	: " << attendent2.getTotalIdleTime() / config.simulationRepeats << endl;
		cout << "	Total Idle Time For Attendent 2b: " << attendent2b.getTotalIdleTime() / config.simulationRepeats << endl;
		if (config.addAttendent)
			cout << "	Total Idle Time For Attendent 2c: " << attendent2c.getTotalIdleTime() / config.simulationRepeats << endl;
		cout << "	Total Secondary Service Done	: " << exitQueue2.getSize() / config.simulationRepeats << endl;
		cout << "	Secondary Service Per Attendent	: " << (exitQueue2.getSize() / 5) / config.simulationRepeats << endl;
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