#include "SimulationCore.h"
#include "LogNormal.h"
#include "Normal.h"
#include "UniformNormal3.h"
#include "Generator.h"
#include "Service.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <tclap/CmdLine.h>

using namespace std;

typedef struct _simulCongif
{
	bool addAttendant;
	bool useDoubleAssignmentIncoming;
	bool useDoubleAssignmentOutgoing;

	unsigned long simulationTime;
	unsigned long simulationRepeats;

	unsigned int seed;

	string fileName;
	bool saveToFile;
	bool pause;

	bool incomingFreeService;
	unsigned int minFreeServices;

}SimulConfig;

using namespace TCLAP;

void getConfigInfo(SimulConfig &config, int argc, char** argv)
{
	try {

		CmdLine cmd("Easy Simul <Examples> - Call Center Simulation", ' ', "1.0");

		ValueArg<unsigned int> seedArg("s", "seed", "Set simulation seed.", false, 5489, "uint");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(seedArg);

		SwitchArg xSwitch("x", "rule", "Active incoming free service rule ('m' services must be free to wait for incoming calls). [needs -i and/or -o]", false);
		cmd.add(xSwitch);

		ValueArg<unsigned int> minFreeServicesArg("m", "min", "Set minimum number of services.", false, 1, "uint");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(minFreeServicesArg);

		SwitchArg pauseSwitch("p", "pause", "Pause cmd at the end.", false);
		cmd.add(pauseSwitch);

		SwitchArg newAttendantSwitch("a", "add", "Add 1 attendant.", false);
		cmd.add(newAttendantSwitch);

		SwitchArg outgoingSwitch("o", "outgoing", "Set outgoing service for all attendants.", false);
		cmd.add(outgoingSwitch);

		SwitchArg incomingSwitch("i", "incomig", "Set incoming service for all attendants.", false);
		cmd.add(incomingSwitch);

		ValueArg<unsigned long> repeatArg("r", "repeat", "Set number of times to repeat the simulation.", false, 1, "ulong");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(repeatArg);

		ValueArg<unsigned long> timeArg("t", "time", "Set simulation time in minutes.", false, 600, "ulong");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(timeArg);
		
		ValueArg<string> fileArg("f", "file", "Set output file.", false, "filename.txt", "string");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(fileArg);

		cmd.parse(argc, argv);

		config.addAttendant = newAttendantSwitch.getValue();
		config.seed = seedArg.getValue();
		config.simulationRepeats = repeatArg.getValue();
		config.simulationTime = timeArg.getValue();
		config.useDoubleAssignmentIncoming = incomingSwitch.getValue();
		config.useDoubleAssignmentOutgoing = outgoingSwitch.getValue();
		config.pause = pauseSwitch.getValue();
		config.incomingFreeService = xSwitch.getValue();
		config.saveToFile = fileArg.isSet();
		config.minFreeServices = minFreeServicesArg.getValue();
		if (config.saveToFile)
		{
			config.fileName = fileArg.getValue();
		}
	}
	catch (ArgException &e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}
}


int main(int argc, char** argv)
{
	stringstream outStream;
	outStream.clear();

	SimulConfig config;
	
	getConfigInfo(config, argc, argv);

	if (argc==2)
		if (argv[1][1] == 'h' || strcmp(argv[1], "--help") == 0)
		{
			exit(1);
		}

	Service::minFreeServices = config.minFreeServices;

	RandomController::init(config.seed);

	double outCallAnswered = 13, outCallBusy = 2.5, outCallMissed = 4;
		
	LogNormal lnDist(2.0179,1.2223); //Arrival Distribution
	Normal nDist(15.714, 11.604);
	unsigned int prob[3] = { 3, 39, 58 };
	double m3[3] = { outCallBusy, outCallAnswered, outCallMissed };//{ 1, 8, 2 };//
	double s3[3] = { 0.005, 1, 0.005 };
	UniformNormal3 un3Dist(prob, m3, s3);

	Queue incomingCalls(EntryQueue), exitQueue(ExitQueue), exitQueue2(ExitQueue), emptyQueue;

	Generator inGen(&lnDist);
	inGen.setOutputQueue(&incomingCalls);
	inGen.setTimeLimit(config.simulationTime);



	//----------------------------- INCOMING CALLS -----------------------------
	Service attendant[10] = { 
		Service(&nDist), Service(&nDist), Service(&nDist), Service(&nDist), Service(&nDist),
		Service(&nDist), Service(&nDist), Service(&nDist), Service(&nDist), Service(&nDist)};

	attendant[0].setInputQueue(&incomingCalls);
	attendant[0].setOutputQueue(&exitQueue);
	attendant[0].setTurn(0, 6 * 60);
	if (config.useDoubleAssignmentOutgoing)
	{
		attendant[0].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	}

	attendant[1].setInputQueue(&incomingCalls);
	attendant[1].setOutputQueue(&exitQueue);
	attendant[1].setTurn(0, 6 * 60);
	if (config.useDoubleAssignmentOutgoing)
	{ 
		attendant[1].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	}
		
	
	attendant[2].setInputQueue(&incomingCalls);
	attendant[2].setOutputQueue(&exitQueue);
	attendant[2].setTurn(4 * 60, -1);
	if (config.useDoubleAssignmentOutgoing)
	{
		attendant[2].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	}

	attendant[3].setInputQueue(&incomingCalls);
	attendant[3].setOutputQueue(&exitQueue);
	attendant[3].setTurn(4 * 60, -1);
	if (config.useDoubleAssignmentOutgoing)
	{
		attendant[3].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	}
	
	attendant[9].setInputQueue(&incomingCalls);
	attendant[9].setOutputQueue(&exitQueue);
	attendant[9].setTurn(0, 6*60);
	if (config.useDoubleAssignmentOutgoing)
	{
		attendant[9].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	}


	bool activateFreeServiceRule = ((config.useDoubleAssignmentIncoming || config.useDoubleAssignmentOutgoing) && config.incomingFreeService);

	if (activateFreeServiceRule)
	{
		attendant[0].activeFreeIncomingServiceRule();
		attendant[1].activeFreeIncomingServiceRule();
		attendant[2].activeFreeIncomingServiceRule();
		attendant[3].activeFreeIncomingServiceRule();
		attendant[9].activeFreeIncomingServiceRule();
	}

	//----------------------------- OUTGOING CALLS -----------------------------

	Queue *q = &emptyQueue;

	if (config.useDoubleAssignmentIncoming)
	{
		q = &incomingCalls;
		if (activateFreeServiceRule)
		{
			attendant[4].activeFreeIncomingServiceRule();
			attendant[5].activeFreeIncomingServiceRule();
			attendant[6].activeFreeIncomingServiceRule();
			attendant[7].activeFreeIncomingServiceRule();
			attendant[8].activeFreeIncomingServiceRule();
		}
	}

	attendant[4].setInputQueue(q);
	attendant[4].setOutputQueue(&exitQueue);
	attendant[4].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendant[4].setTurn(0, 6 * 60);

	attendant[5].setInputQueue(q);
	attendant[5].setOutputQueue(&exitQueue);
	attendant[5].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendant[5].setTurn(0, 6 * 60);

	attendant[6].setInputQueue(q);
	attendant[6].setOutputQueue(&exitQueue);
	attendant[6].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendant[6].setTurn(4 * 60, -1);

	attendant[7].setInputQueue(q);
	attendant[7].setOutputQueue(&exitQueue);
	attendant[7].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendant[7].setTurn(4 * 60, -1);

	attendant[8].setInputQueue(q);
	attendant[8].setOutputQueue(&exitQueue);
	attendant[8].setSecondaryService(&un3Dist, &exitQueue2, config.simulationTime);
	attendant[8].setTurn(4 * 60, -1);

	//----------------------------- CORE -----------------------------

	SimulationCore core;

	core.add(&inGen);

	for (int i = 0; i < 9; i++)
		core.add(&attendant[i]);

	if (config.addAttendant)
		core.add(&attendant[9]);

	core.setRepeat(config.simulationRepeats);

	core.run();

	vector<Entity*> eList = exitQueue.emptyToVector();
	vector<Entity*> eList2 = exitQueue2.emptyToVector();

	outStream << endl << "------------------- Total Output <of " << config.simulationRepeats << " simulation(s)> -------------------" << endl << endl;
	outStream << "	Incoming Clients		: " << inGen.entityCounter << endl;
	
	for (int i = 0; i < 10; i++)
	{
		if (config.addAttendant || i < 9)
			outStream << "	Idle Time For Attendant " << i + 1 << "	: " << attendant[i].getTotalIdleTime() << " minutes" << endl;
	}

	outStream << "	Secondary Service Done		: " << eList2.size() << endl;
	outStream << "	Secondary Service / Attendants	: " << eList2.size() / ((config.useDoubleAssignmentOutgoing) ? (((config.addAttendant) ? 10 : 9)) : 5) << endl;

	if (config.simulationRepeats == 1)
	{
		outStream << "	End Time			: " << core.getCurrentTime() << " minutes" << endl;
	}
	else
	{
		outStream << endl << "------------------- Median Output <of " << config.simulationRepeats << " simulation(s)> -------------------" << endl << endl;
		outStream << "	Incoming Clients / Simulation	: " << inGen.entityCounter / config.simulationRepeats << endl;

		for (int i = 0; i < 10; i++)
		{
			if (config.addAttendant || i < 9)
				outStream << "	Idle Time For Attendant " << i + 1 << "	: " << attendant[i].getTotalIdleTime() / config.simulationRepeats << " minutes" << endl;
		}

		//if (config.addAttendant)
		//	outStream << "	Total Idle Time For Attendant 10	: " << attendant[9].getTotalIdleTime() / config.simulationRepeats << " minutes" << endl;

		outStream << "	Secondary Service Done		: " << eList2.size() / config.simulationRepeats << endl;
		outStream << "	Secondary Service / Attendants	: " << (eList2.size() / ((config.useDoubleAssignmentOutgoing) ? (((config.addAttendant)?10:9)) : 5)) / config.simulationRepeats << endl;
	}

	outStream << endl << "------------------- ++++++ Output <of " << config.simulationRepeats << " simulation(s)> -------------------" << endl << endl;

	outStream << "	----- Incoming Clients Statistics -----" << endl;
	//Maior tempo de espera na fila
	outStream << "	Incoming Call Max Time In Queue	: " << IncomingEntity::getMaxTimeInQueue() << " minutes" << endl;
	if (config.simulationRepeats > 1)
		outStream << "	 *Average Max Time In Queue	: " << (double)IncomingEntity::getMaxTimeInQueueSum() / (double)config.simulationRepeats << " minutes" << endl;

	//Tempo Médio de um cliente na Fila
	double cQueueWaitingTime = 0;
	unsigned int clientsWaitedInQueue = 0;
	double cServiceTime = 0;
	double cSystemTime = 0;
	for (size_t i = 0; i < eList.size(); i++)
	{
		unsigned long long tQ = ((IncomingEntity*)eList[i])->getTotalTimeInQueue();
		unsigned long long tSys = ((IncomingEntity*)eList[i])->getTotalTimeInSystem();
		unsigned long long tSer = ((IncomingEntity*)eList[i])->getTotalTimeInService();

		//if (i < 10)
		//	outStream << i << " - [DEBUG] - " << tSys << " = " << tQ << " + " << tSer << endl;
		cSystemTime += tSys;
		cServiceTime += tSer;
		cQueueWaitingTime += tQ;

		if (tQ > 0)
		{
			clientsWaitedInQueue++;
		}
	}

	cQueueWaitingTime /= eList.size();
	cServiceTime /= eList.size();
	cSystemTime /= eList.size();

	double clientInQueue = 100*((double)clientsWaitedInQueue / (double)eList.size());
	outStream << "	Probability of Client in Queue	: " << setprecision(3) << clientInQueue << "%" << endl;
	outStream << "	Average Queue Waiting Time	: " << setprecision(4) << cQueueWaitingTime << " minutes" << endl;
	outStream << "	Average Service Time	: " << setprecision(4) << cServiceTime << " minutes" << endl;
	outStream << "	Average Time in System	: " << setprecision(4) << cSystemTime << " minutes" << endl;

	//Probabilidade de tamanho de fila
	unsigned long totalSimulationTime = incomingCalls.getTotalSimulationTime();
	std::map<unsigned long, unsigned long long> queueTimePerSize = incomingCalls.getTimePerSize();
	double medianClientsInQueue = 0;
	outStream << "	Probability of Queue Size	 " << endl;
	for (std::map<unsigned long, unsigned long long>::iterator it = queueTimePerSize.begin(); it != queueTimePerSize.end(); ++it)
	{
		medianClientsInQueue += ((double)(it->first*it->second) / (double)totalSimulationTime);
		double p = ((double)it->second / (double)totalSimulationTime) * 100;
		outStream << "			Queue Size = " << it->first << "	: <" << ((p >= 10) ? "" : "0") << setprecision((p >= 10) ? 6 : ((p < 1) ? 4 : 5)) << p << "%>" << endl;

	}

	outStream << "	Average Expected Queue Size	: " << medianClientsInQueue << endl;

	outStream << "	----- Outgoing Clients Statistics -----" << endl;

	double cOutServiceTime = 0;
	double cOutSystemTime = 0;
	double answeredCalls = 0, busyCalls = 0, missedCalls = 0;
	for (size_t i = 0; i < eList2.size(); i++)
	{
		unsigned long long tSys = ((OutgoingEntity*)eList2[i])->getTotalTimeInSystem();
		unsigned long long tSer = ((OutgoingEntity*)eList2[i])->getTotalTimeInService();

		//if (i < 20)
		//	outStream << i << " - [DEBUG] - " << tSys << " = " << tSer << endl;

		if (tSys <= outCallBusy + 0.5)
		{
			busyCalls++;
		}
		else if (tSys <= outCallMissed + 0.5)
		{
			missedCalls++;
		}
		else
		{
			answeredCalls++;
		}

		cOutSystemTime += tSys;
		cOutServiceTime += tSer;
	}

	cOutServiceTime /= eList2.size();
	cOutSystemTime /= eList2.size();
	busyCalls /= eList2.size();
	busyCalls *= 100;
	missedCalls /= eList2.size();
	missedCalls *= 100;
	answeredCalls /= eList2.size();
	answeredCalls *= 100;

	outStream << "	Average Service Time	: " << setprecision(4) << cOutServiceTime << " minutes" << endl;
	outStream << "	Average Time in System	: " << setprecision(4) << cOutSystemTime << " minutes" << endl;

	outStream << "	Probability per Call Type	" << endl;
	outStream << "			Busy Calls		: <" << ((busyCalls >= 10) ? "" : "0") << setprecision((busyCalls >= 10) ? 6 : ((busyCalls < 1) ? 4 : 5)) << busyCalls << "%>" << endl;
	outStream << "			Missed Calls		: <" << ((missedCalls >= 10) ? "" : "0") << setprecision((missedCalls >= 10) ? 6 : ((missedCalls < 1) ? 4 : 5)) << missedCalls << "%>" << endl;
	outStream << "			Answered Calls		: <" << ((answeredCalls >= 10) ? "" : "0") << setprecision((answeredCalls >= 10) ? 6 : ((answeredCalls < 1) ? 4 : 5)) << answeredCalls << "%>" << endl;


	outStream << "	----- Other Statistics -----" << endl;

	outStream << "	Attendant Busy/Free Time	 " << endl;
	for (int i = 0; i < 10; i++)
	{
		unsigned long long tIdle = attendant[i].getTotalIdleTime();
		unsigned long long tTotal = attendant[i].getTotalSimulationTime();
		double b = 100*(((double)tTotal - (double)tIdle) / (double)tTotal), f = 100*((double)tIdle / (double)tTotal);
		if (config.addAttendant || i < 9)
		{
			outStream << "			Attendant " << i + 1 << "	: <Busy: " << setprecision(3) << b << "%>" << "	<Free:" << setprecision(3) << f << "%>" << endl;

			//outStream << "					  Attendant " << i + 1 << endl;
			//outStream << "						" << "  <Busy: " << setprecision(3) << b << "%>" << endl;
			//outStream << "						" << "  <Free:" << setprecision(3) << f << "%>" << endl;
		}
	}

	outStream << "	Attendant Service Type " << endl;
	for (int i = 0; i < 10; i++)
	{
		unsigned long long tIn = attendant[i].getEntityIncomingCounter();
		unsigned long long tOut = attendant[i].getEntityOutgoingCounter();
		unsigned long long tTotal = tIn+tOut;
		double in = 100 * ((double)tIn / (double)tTotal), out = 100 * ((double)tOut / (double)tTotal);
		if (config.addAttendant || i < 9)
		{
			outStream << "			Attendant " << i + 1 << "	: <Incoming: " << setprecision(3) << in << "%>" << "	<Outgoing:" << setprecision(3) << out << "%>" << endl;
		}
	}

	outStream << endl;

	cout << outStream.str();

	if (config.saveToFile)
	{
		ofstream outFile;

		outFile.open(config.fileName);

		if (outFile.is_open())
		{
			outFile << "Command: ";
			for (size_t i = 0; i < argc; i++)
			{
				outFile << argv[i] << " ";
			}
			outFile << endl;
			outFile << outStream.str();
			outFile.close();
		}
	}

	if (config.pause)
		system("pause");

	//RandomController rc;

	//ofstream file;

	//file.open("output.txt");

	//for (int i = 0; i < 130; i++)
	//	file << rc.getRandomLogNormal() << endl;

	//file.close();

	return 0;
}