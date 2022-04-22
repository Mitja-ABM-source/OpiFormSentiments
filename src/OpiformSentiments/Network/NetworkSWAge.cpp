#include <random>
#include <algorithm>
#include <fstream>

#include "../Agent/AgentBase.h"

#include "NetworkSWAge.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

NetworkSWAge::NetworkSWAge(int anNeighbors, const double & adbRewiringProb) : NetworkSW(anNeighbors, adbRewiringProb) {
}

//-------------------------------------------------------------------------------------

NetworkSWAge::~NetworkSWAge() {
}

//-------------------------------------------------------------------------------------

bool NetworkSWAge::generateNetwork(vector<AgentBase*> * apvecAgents)	{
	NetworkSW::generateRegularLattice(apvecAgents);
	NetworkSW::networkRewiring(apvecAgents, m_dbRewiringProb);
	return true;
}

//-------------------------------------------------------------------------------------

void NetworkSWAge::networkRewiring(std::vector<AgentBase *> * apvecAgents, const double & adbProb) {

	//Age in months => in years
	std::map<int/*Age*/, std::vector<int>/*IDs*/> LUT;
	for (const auto * pA : *apvecAgents) {
		LUT[(int)((double)pA->getAge() / 12.0)].push_back(pA->getID());
	}

	std::uniform_real_distribution<double> dis;
	//Add age attachment
	for (auto * pAgent : *apvecAgents) {

		//Age
		int nAge = (int)((double)pAgent->getAge() / 12.0);

		std::vector<int> nvecIDs;
		for (int nI = -5; nI < 5; ++nI) {
			auto vecIDs = LUT[nAge + nI];
			if (vecIDs.empty() == false) {
				nvecIDs.reserve(nvecIDs.size() + vecIDs.size()); // Reserve space first
				nvecIDs.insert(
					nvecIDs.end(), 
					std::make_move_iterator(vecIDs.begin()),
					std::make_move_iterator(vecIDs.end())
					);
				vecIDs.swap(std::vector<int>());
			}
		}

		//AMon the generation pick N at random
		int nGeneration = 10, nSize = nvecIDs.size();
		if (nSize <= nGeneration) {
			for (int nI = 0; nI < nSize; ++nI) {
				//Make new connections
				AgentBase * pA = (*apvecAgents)[nvecIDs[nI]];
				pA->addAdjacentAgent(pAgent);
				pAgent->addAdjacentAgent(pA);
			}
		}
		else {
			int nI = 0;

			std::uniform_int_distribution<> uidis(0, nSize - 1);
			while (nI < 10) {
				AgentBase* pAgentNew = pAgent;
				while (pAgent->isMember(pAgentNew) == true) {
					pAgentNew = (*apvecAgents)[nvecIDs[uidis(gen)]];
				}
				pAgentNew->addAdjacentAgent(pAgent);
				pAgent->addAdjacentAgent(pAgentNew);
				++nI;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkSWAge::step(std::vector<AgentBase *> * apvecAgents) {
	return true;
}