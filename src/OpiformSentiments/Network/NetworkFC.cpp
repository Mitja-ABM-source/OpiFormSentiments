#include <random>

#include "../Agent/AgentBase.h"

#include "NetworkFC.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd() /*10*/);
}


NetworkFC::NetworkFC() : NetworkAbstract() {
}

NetworkFC::~NetworkFC() {}


bool NetworkFC::generateNetwork(std::vector<AgentBase *> * apvecAgents)	{
	//Not generated because it accesses all agents
	return true;
}

//---------------------------------------------------------------------------------------------------------------------

bool NetworkFC::step(std::vector<AgentBase *> * apvecAgents) {

	int nSize = apvecAgents->size();

	//Iterate through agents
	for (AgentBase * pAgent : *apvecAgents) {
		AgentBase* pAgentAdjacent = nullptr;

		std::uniform_int_distribution<> dis(0, apvecAgents->size()-1);
		int nID = dis(gen);

		pAgentAdjacent = (*apvecAgents)[nID];

		if (pAgentAdjacent == nullptr)
			continue;

		//Choose a topic
		Opinion::OpinionTopic t = Opinion::getTopic();

		//Store opinions before a change
		Opinion::OpinionPosition opinionAgent = pAgent->getTopicPosition(t);
		Opinion::OpinionPosition opinionAdjacent = pAgentAdjacent->getTopicPosition(t);

		double dbMu = pAgent->getMu();
		double dbAdjMu = pAgentAdjacent->getMu();

		if (pAgent->shouldUpdate(t, opinionAdjacent, dbMu)) {

			Opinion::OpinionPosition op = opinionAgent + dbMu*(opinionAdjacent - opinionAgent);
			pAgent->setOpinionTopicPosition(t, op);
		}

		if (pAgentAdjacent->shouldUpdate(t, opinionAgent, dbAdjMu)) {
			Opinion::OpinionPosition op = opinionAdjacent + dbAdjMu*(opinionAgent - opinionAdjacent);
			pAgentAdjacent->setOpinionTopicPosition(t, op);
		}

		//Increase agent's age
		pAgent->setAge(pAgent->getAge() + 1);

	}

	return true;
}