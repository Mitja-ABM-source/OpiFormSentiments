#include <random>

#include "../Agent/AgentBase.h"

#include "NetworkER.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

NetworkER::NetworkER(const double & adbConnectionProb) : NetworkAbstract(), m_dbConnectionProb(adbConnectionProb) {
}

NetworkER::~NetworkER() {}


bool NetworkER::generateNetwork(std::vector<AgentBase*> * apvecAgents)	{

	int nSize = apvecAgents->size();

	double dbC = 1.5;
	const double dbProb = dbC * (1.0 / nSize) * log(nSize);

	std::uniform_real_distribution<double> dis;

	for (int nI = 0; nI< nSize; ++nI) {

		AgentBase* pAgent = (*apvecAgents)[nI];

		for (int nJ = 0; nJ < nSize; ++nJ) {
			if (dbProb > dis(gen) && nI != nJ) {
				AgentBase* pAgentAdjacent = (*apvecAgents)[nJ];

				bool bRes = pAgent->isMember(pAgentAdjacent);
				if (bRes == false)
					pAgent->addAdjacentAgent(pAgentAdjacent);
			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkER::step(std::vector<AgentBase *> * apvecAgents) {

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