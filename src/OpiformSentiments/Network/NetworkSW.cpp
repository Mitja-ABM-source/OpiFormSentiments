#include <random>
#include <algorithm>

#include "../Agent/AgentBase.h"

#include "NetworkSW.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

NetworkSW::NetworkSW(int anNeighbors, const double & adbRewiringProb) : NetworkAbstract(), m_nNeighbors(anNeighbors), m_dbRewiringProb(adbRewiringProb) {
}

//-------------------------------------------------------------------------------------

NetworkSW::~NetworkSW() {
}

//-------------------------------------------------------------------------------------

bool NetworkSW::generateRegularLattice(std::vector<AgentBase *> * apvecAgents) {
	gen.seed(10);

	int nSize = apvecAgents->size();
	vector<int> vecIDs(nSize);
	for (int nI = 0; nI < nSize; ++nI)
		vecIDs[nI] = nI;

	uniform_real_distribution<double> dis;
	random_shuffle(vecIDs.begin(), vecIDs.end());

	for (int nI = 0; nI< nSize; ++nI) {
		int nnI = vecIDs[nI];
		AgentBase * pAgent = (*apvecAgents)[nnI];

		for (int nJ = 1; nJ <= m_nNeighbors >> 1; ++nJ) {
			int nID = nI + nJ; //half connections to each side

			if (nID >= nSize)
				nID -= nSize;

			if (vecIDs[nID] == nnI)
				continue;

			AgentBase * pAgentAdj = (*apvecAgents)[vecIDs[nID]];
						
			pAgent->addAdjacentAgent(pAgentAdj);
			pAgentAdj->addAdjacentAgent(pAgent);
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------

bool NetworkSW::generateNetwork(vector<AgentBase*> * apvecAgents)	{

	generateRegularLattice(apvecAgents);
	networkRewiring(apvecAgents, m_dbRewiringProb);

	return true;
}

//-------------------------------------------------------------------------------------

void NetworkSW::networkRewiring(std::vector<AgentBase *> * apvecAgents, const double & adbProb) {
	std::uniform_real_distribution<double> dis;
	int nSize = apvecAgents->size();

	for (int nI = 0; nI < nSize; ++nI) {
		AgentBase* pAgent = (*apvecAgents)[nI];

		//Check rewiring for each connection
		for (int nJ = 0; nJ < pAgent->getNeighborhoodSize(); ++nJ) {
			double dbProb = dis(gen);

			if (dbProb < adbProb) {
				//Rewire
				std::uniform_int_distribution<> uidis(0, nSize - 1);

				//New connection shouldn't be already within the neighborhood
				AgentBase* pAgentReplacement = pAgent;
				while (pAgent->isMember(pAgentReplacement) == true) {
					pAgentReplacement = (*apvecAgents)[uidis(gen)];
				}

				//Make new connections
				pAgentReplacement->addAdjacentAgent(pAgent);
				pAgent->addAdjacentAgent(pAgentReplacement);

				//Remove old connections
				AgentBase* pAgentOld = (AgentBase*)pAgent->getNeighbor(nJ);
				pAgent->removeAgent(pAgentOld);
				pAgentOld->removeAgent(pAgent);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkSW::step(std::vector<AgentBase *> * apvecAgents) {

	int nSize = apvecAgents->size();

	//Iterate through agents
	for (AgentBase * pAgent : *apvecAgents) {
		//Choose a topic
		Opinion::OpinionTopic t = Opinion::getTopic();

		int nRes = (*m_OpiformModel)(pAgent, t);

		//Increase agent's age
		pAgent->setAge(pAgent->getAge()+1);
	}

	return true;
}