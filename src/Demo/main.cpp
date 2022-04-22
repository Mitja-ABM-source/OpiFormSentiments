#include <string>
#include <iostream>
#include <chrono>

#include "Utils/Writer.h"

#include "Network/NetworkAbstract.h"

#include "Game/Game.h"

using namespace std;
using namespace opiform;

int main(int argc, char * argv[]) {
	/*
		Params:
		- nAgents:		number of agents used
		- tIerations:	number of iterations per repetition
		- nRepetitions:	number of repetitons per network
		- netType:		type of network
		{
		SW (Watts-Strogatz),
		FC (Fully-connected),
		ER (Erdos–Renyi),
		BA (Barabasi-Albert),
		CM (Communities)
		}

		Additional params:
		1.) output folder = argv[1] or "./" if not given
		*/

	if (argc == 2)
		Utils::setFolder(argv[1]);

	else
		Utils::setFolder("C:\\4Znanstveno_raziskovanje\\clanki_in_konference\\0\\JASSS-Opinion-Games\\NoveSimulacije\\Release\\");
	    // Utils::setFolder("C:\\4Znanstveno_raziskovanje\\clanki_in_konference\\0\\JASSS-Opinion-Games\\NoveSimulacije\\Debugging\\");

	const int nAgents = 300;
	const time_t tIterations = 1001;
	const int nRepetitions = 1;
	double dbGammaMeanMax = 0.95; // persistence score ceiling (agent's)
	double dbKappaMeanMax = 0.95; // persistence score parameter kappa
	double dbThetaMeanMax = 0.95;
 // opinion threshold parameter theta (agent-specific)
	double dbMuMean = 0.85; // bounded confidence parameter lambda (agent-specific)

	Game::registerStatics();

	auto t0 = std::chrono::system_clock::now();
	system("color 0A");
	char a = 177, b = 219;
	for (int i = 0; i < 49; i++)
	{ std::cout << a; }
	std::cout << "\r";

	for (int nI = 0; nI < nRepetitions; ++nI) {
		int nnI = 0;
		for (double dbKappaMean = 0.1; dbKappaMean <= dbKappaMeanMax; dbKappaMean += 0.1) {
			for (double dbGammaMean = 0.1; dbGammaMean <= dbGammaMeanMax; dbGammaMean += 0.1) {
				for (double dbThetaMean = 0.1; dbThetaMean <= dbThetaMeanMax; dbThetaMean += 0.1) {
					string strName = "results_SB_avgKappa_" + to_string(dbKappaMean) + "_avgGamma_" + to_string(dbGammaMean) + "_avgTheta_" + to_string(dbThetaMean) + "_.txt";
					Game game(strName.c_str());
					if (game.init(nAgents, dbMuMean, dbKappaMean, dbGammaMean, dbThetaMean) == false)
						continue;

					NetworkAbstractParams np;
					np.m_dbConnectionProb = 0.075;
					np.m_dbRewiringProb = 0.15;
					np.m_nConnectedNodes = 0;
					np.m_nNeighbors = 8;
					np.m_OpinionFormationModelType = OpinionFormationModel::DW;
					NetworkType netType = NetworkType::SW;

					if (game.initNetwork(netType, &np) == false)
						continue;

					game.runGame(tIterations);
					if (nnI % 15 == 0)
					{ std::cout << b; }
					nnI += 1;
				}
			}
		}
	}

	auto t1 = chrono::system_clock::now();
	std::chrono::duration<double> diff = t1-t0;
	std::cout << "\n\n\n";
	std::cout << "done" << '\t' << diff.count() << std::endl;

	return 0;
}
