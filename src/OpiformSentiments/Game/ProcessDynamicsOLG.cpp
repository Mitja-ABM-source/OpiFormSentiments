#include <iostream>
#include <random>

#include "../Agent/AgentBase.h"
#include "ProcessDynamicsBase.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}

void ProcessDynamics<ProcessDynamicsType::OLG>::update(std::queue<int> & aqDeceased) {

	if (aqDeceased.empty())
		return;

	std::vector<Opinion::mapOpinionTopicPosition> vecYoung;
	vecYoung.reserve(m_pvecAgents->size());
	for (auto & el : *m_pvecAgents) {
		if (el->getAge() < (++opiform::mapLifeTable.cbegin())->first)
			vecYoung.push_back(*(el->getBelieves()));
	}

	std::uniform_int_distribution<> dis(0,vecYoung.size());

	while (aqDeceased.empty() == false) {
		int nID = aqDeceased.front();
		aqDeceased.pop();

		AgentBase * pAgent = (*m_pvecAgents)[nID];

		int nAgent = dis(gen);
		pAgent->setBelieves(*(*m_pvecAgents)[nAgent]->getBelieves());
		pAgent->setAge(mapLifeTable.cbegin()->first);
		pAgent->resetAdjacentAgentProfiles();

	}
}

//=========================================================================================================
