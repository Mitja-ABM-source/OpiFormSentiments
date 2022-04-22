#include <random>
#include <queue>

#include "../Agent/AgentBase.h"

#include "NetworkCM.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

NetworkCM::NetworkCM(int anGroupSize) : NetworkAbstract(), m_nGroupSize(anGroupSize) {
}

//-------------------------------------------------------------------------------------

NetworkCM::~NetworkCM() {
}

//-------------------------------------------------------------------------------------

bool NetworkCM::generateNetwork(vector<AgentBase *> * apvecAgents)	{

	int nSize = apvecAgents->size();

	vector<int> nvecIDs(nSize);
	for (int nI = 0; nI < nSize; ++nI)
		nvecIDs[nI] = nI;

//	random_shuffle(nvecIDs.begin(), nvecIDs.end());

	vector<vector<int> > vecGroups;

	//1. each group is a component
	int nI = 0;
	while (nI < nSize) {

		vector<int> vecGroup;
		for (int nZ = nI; nZ < nI + m_nGroupSize && nZ < nSize; ++nZ) {

			AgentBase* pAgent = (*apvecAgents)[nvecIDs[nZ]];
			vecGroup.push_back(nvecIDs[nZ]);

			for (int nJ = nZ + 1; nJ < nI + m_nGroupSize && nJ < nSize; ++nJ) {

				AgentBase* pAgentAdj = (*apvecAgents)[nvecIDs[nJ]];
				pAgent->addAdjacentAgent(pAgentAdj);
				pAgentAdj->addAdjacentAgent(pAgent);
			}
		}
		vecGroups.push_back(vecGroup);
		nI += m_nGroupSize;
	}

	//2. connect components in a circle with one connection

	nSize = vecGroups[0].size();
	uniform_real_distribution<> dis;
	for (int nI = 1; nI < vecGroups.size(); ++nI) {
		int nID = (int)nSize*dis(gen);
		AgentBase * pA0 = (*apvecAgents)[vecGroups[nI - 1][nID]];
			
		nSize = vecGroups[nI].size();
		nID = (int)nSize*dis(gen);
		AgentBase * pA1 = (*apvecAgents)[vecGroups[nI][nID]];

		pA0->addAdjacentAgent(pA1);
		pA1->addAdjacentAgent(pA0);
	}

	//Finally, connect first and last group
	int nID = (int)vecGroups[0].size()*dis(gen);
	AgentBase * pA0 = (*apvecAgents)[vecGroups[0][nID]];

	nSize = vecGroups.back().size();
	nID = (int)nSize*dis(gen);
	AgentBase * pA1 = (*apvecAgents)[vecGroups.back()[nID]];

	pA0->addAdjacentAgent(pA1);
	pA1->addAdjacentAgent(pA0);

	return true;
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkCM::step(std::vector<AgentBase *> * apvecAgents) {

	int nSize = apvecAgents->size();

	//Iterate through agents
	for (AgentBase * pAgent : *apvecAgents) {

		//Choose a topic
		Opinion::OpinionTopic t = Opinion::getTopic();

		int nRes = (*m_OpiformModel)(pAgent, t);

		//Increase agent's age
		pAgent->setAge(pAgent->getAge() + 1);
	}

	return true;
}