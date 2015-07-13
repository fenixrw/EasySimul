#include "SimulationCore.h"
#include "LogNormal.h"
#include "Normal.h"
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

	unsigned long simulationTime;
	unsigned long simulationRepeats;

	unsigned int seed;

	string fileName;
	bool saveToFile;
	bool pause;

}SimulConfig;

using namespace TCLAP;

void getConfigInfo(SimulConfig &config, int argc, char** argv)
{
	try {

		CmdLine cmd("Easy Simul <Examples> - Simple Queue System", ' ', "1.0");

		ValueArg<unsigned int> seedArg("s", "seed", "Set simulation seed.", false, 5489, "uint");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(seedArg);

		SwitchArg pauseSwitch("p", "pause", "Pause cmd at the end.", false);
		cmd.add(pauseSwitch);

		ValueArg<unsigned long> repeatArg("r", "repeat", "Set number of times to repeat the simulation.", false, 1, "ulong");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(repeatArg);

		ValueArg<unsigned long> timeArg("t", "time", "Set simulation time in minutes.", false, 600, "ulong");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(timeArg);
		
		ValueArg<string> fileArg("f", "file", "Set output file.", false, "filename.txt", "string");// ("n", "name", "Name to print", true, "homer", "string");
		cmd.add(fileArg);

		cmd.parse(argc, argv);

		config.seed = seedArg.getValue();
		config.simulationRepeats = repeatArg.getValue();
		config.simulationTime = timeArg.getValue();
		config.pause = pauseSwitch.getValue();
		config.saveToFile = fileArg.isSet();
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
	
	RandomController::init(config.seed);
	
		
	LogNormal lnDist(2,1.23); //Arrival Distribution
	Normal nDist(15, 11);
	Normal nDist2(6.9, 2.5);

	Queue incomingQueue(EntryQueue), innerQueue(NormalQueue), exitQueue(ExitQueue);

	Generator inGen(&lnDist);
	inGen.setOutputQueue(&incomingQueue);
	inGen.setTimeLimit(config.simulationTime);
	

	//----------------------------- SERVICES -----------------------------
	Service attendant[4] = { Service(&nDist), Service(&nDist), Service(&nDist2), Service(&nDist2) };

	attendant[0].setInputQueue(&incomingQueue);
	attendant[0].setOutputQueue(&innerQueue);
	attendant[0].setTurn(0, config.simulationTime/2);

	attendant[1].setInputQueue(&incomingQueue);
	attendant[1].setOutputQueue(&innerQueue);
	attendant[1].setTurn(config.simulationTime / 3, -1);
	
	attendant[2].setInputQueue(&innerQueue);
	attendant[2].setOutputQueue(&exitQueue);
	attendant[2].setTurn(0, config.simulationTime / 2);

	attendant[3].setInputQueue(&innerQueue);
	attendant[3].setOutputQueue(&exitQueue);
	attendant[3].setTurn(config.simulationTime / 3, -1);
	
	//----------------------------- CORE -----------------------------

	SimulationCore core;

	core.add(&inGen);

	for (int i = 0; i < 4; i++)
		core.add(&attendant[i]);


	for (size_t i = 0; i < config.simulationRepeats; i++)
	{
		//Run Simulation
		core.run();

		//Collect and print results
		vector<Entity*> eList = exitQueue.emptyToVector();
		outStream << endl << "------------------- Output <Simulation " << i << "/" << config.simulationRepeats << ")> -------------------" << endl << endl;

		unsigned long long simulationTime = core.getCurrentTime();

		outStream << "	Simulation Time			: " << simulationTime << " minutes" << endl << endl;
	
		outStream << "	----- Clients Statistics -----" << endl;

		outStream << "	Incoming Clients		: " << inGen.entityCounter << endl;
		//Tempo Médio de um cliente na Fila
		double cQueueWaitingTime = 0;
		unsigned int clientsWaitedInQueue = 0;
		double cServiceTime = 0;
		double cSystemTime = 0;
		for (size_t i = 0; i < eList.size(); i++)
		{
			unsigned long long tQ = eList[i]->getTotalTimeInQueue();
			unsigned long long tSys = eList[i]->getTotalTimeInSystem();
			unsigned long long tSer = eList[i]->getTotalTimeInService();
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
		outStream << "	Average Time in System	: " << setprecision(4) << cSystemTime << " minutes" << endl << endl;


		outStream << "	----- Queue Statistics -----" << endl;
		//Probabilidade de tamanho de fila
		std::map<unsigned long, unsigned long long> queueTimePerSize = incomingQueue.getTimePerSize();
		double medianClientsInQueue = 0;
		outStream << "	<<< Incoming Queue [Services 1-2]>>>	 " << endl;
		outStream << "	Probability of Queue Size	 " << endl;
		for (std::map<unsigned long, unsigned long long>::iterator it = queueTimePerSize.begin(); it != queueTimePerSize.end(); ++it)
		{
			medianClientsInQueue += ((double)(it->first*it->second) / (double)simulationTime);
			double p = ((double)it->second / (double)simulationTime) * 100;
			outStream << "			Queue Size = " << it->first << "	: <" << ((p >= 10) ? "" : "0") << setprecision((p >= 10) ? 6 : ((p < 1) ? 4 : 5)) << p << "%>" << endl;

		}

		outStream << "	Average Expected Queue Size	: " << medianClientsInQueue << endl << endl;


		queueTimePerSize = innerQueue.getTimePerSize();
		medianClientsInQueue = 0;
		outStream << "	<<< Inner Queue [Services 3-4]>>>	 " << endl;
		outStream << "	Probability of Queue Size	 " << endl;
		for (std::map<unsigned long, unsigned long long>::iterator it = queueTimePerSize.begin(); it != queueTimePerSize.end(); ++it)
		{
			medianClientsInQueue += ((double)(it->first*it->second) / (double)simulationTime);
			double p = ((double)it->second / (double)simulationTime) * 100;
			outStream << "			Queue Size = " << it->first << "	: <" << ((p >= 10) ? "" : "0") << setprecision((p >= 10) ? 6 : ((p < 1) ? 4 : 5)) << p << "%>" << endl;

		}

		outStream << "	Average Expected Queue Size	: " << medianClientsInQueue << endl << endl;

		outStream << "	----- Service Statistics -----" << endl;

		for (int i = 0; i < 4; i++)
		{
			outStream << "	Idle Time For Service " << i + 1 << "	: " << attendant[i].getIdleTime() << " minutes" << endl;
		}

		outStream << "	Service Busy/Free Time	 " << endl;
		for (int i = 0; i < 4; i++)
		{
			unsigned long long tIdle = attendant[i].getIdleTime();
			unsigned long long tTotal = attendant[i].getSimulationTime();
			double b = 100*(((double)tTotal - (double)tIdle) / (double)tTotal), f = 100*((double)tIdle / (double)tTotal);
			outStream << "			Service " << i + 1 << "	: <Busy: " << setprecision(3) << b << "%>" << "	<Free:" << setprecision(3) << f << "%>" << endl;
		}

		outStream << "	Service Entity Counter " << endl;
		for (int i = 0; i < 4; i++)
		{
			unsigned long long tIn = attendant[i].getEntityCounter();
			outStream << "			Service " << i + 1 << "	: " << tIn << "" << endl;
		}

		outStream << endl;

		//restart the core, nodes and queues (only needed if you enable repeat simulations)
		core.restart();
	} 
	

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

	return 0;
}