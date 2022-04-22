#include <random>
#include <map>

#include "../Agent/AgentBase.h"

#include "NetworkBA.h"

/*
	http://chianti.ucsd.edu/svn/csplugins/trunk/soc/pjmcswee/src/cytoscape/randomnetwork/BarabasiAlbertModel.java
*/

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(/*rd()*/ 10);
}

//---------------------------------------------------------------------------------------

NetworkBA::NetworkBA(int anConnectedNodes) : NetworkAbstract(), m_nConnectedNodes(anConnectedNodes) {
}

//---------------------------------------------------------------------------------------

NetworkBA::~NetworkBA() {}

//---------------------------------------------------------------------------------------

bool NetworkBA::generateNetwork(std::vector<AgentBase *> * apvecAgents)	{
	gen.seed(10);

	int nSize = apvecAgents->size();

	//Track the degree of each node
	std::map<int, int> mapNodesDegree;
	int nLinks = 0;

	//1. Fully connect initial nodes
	for (int nI = 0; nI < m_nConnectedNodes; ++nI) {
		AgentBase* pAgent = (*apvecAgents)[nI];

		for (int nJ = nI+1; nJ < m_nConnectedNodes; ++nJ) {

			AgentBase* pAgentAdjacent = (*apvecAgents)[nJ];
			pAgent->addAdjacentAgent(pAgentAdjacent);
			++mapNodesDegree[pAgent->getID()];	//Increment node degree

			pAgentAdjacent->addAdjacentAgent(pAgent);
			++mapNodesDegree[pAgentAdjacent->getID()];	//Increment node degree
			nLinks += 2;
		}
	}

	//2. Preferential attachement linkage

	int nLinks2Add = m_nConnectedNodes;
	std::uniform_real_distribution<double> dis;

	for (int nI = m_nConnectedNodes; nI < nSize; ++nI) {

		//Finite number of links to add
		for (int nJ = 0; nJ < nLinks2Add; ++nJ) {
			//Try adding the pAgent to all existing nodes
			int nLinksAdded = 0;
			double dbProb = dis(gen);
			double dbProbDegree = 0.0;

			for (int nZ = 0; nZ < nI; ++nZ) {
				dbProbDegree += (double)mapNodesDegree[nZ] / (double)(nLinks - nLinksAdded);

				if (dbProb < dbProbDegree) {
					//Establish link between Agent[nI] and Agent[nZ]
					AgentBase* pAgent = (*apvecAgents)[nZ];
					AgentBase* pAgentAdjacent = (*apvecAgents)[nI];

					bool bIncluded = pAgentAdjacent->isMember(pAgent);
					if (bIncluded == false) {
						pAgentAdjacent->addAdjacentAgent(pAgent);
						pAgent->addAdjacentAgent(pAgentAdjacent);
						++mapNodesDegree[pAgentAdjacent->getID()];
						++mapNodesDegree[pAgent->getID()];

						nLinks += 2;
						nLinksAdded += 2;

						break;
					}
				}
			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkBA::step(std::vector<AgentBase *> * apvecAgents) {

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