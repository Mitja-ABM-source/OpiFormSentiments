#include <random>

#include "../Agent/AgentBase.h"
#include "ProcessDynamicsReset.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;

	std::mt19937 gen(
		rd()
		//		10
		);
}

void ProcessDynamics<ProcessDynamicsType::Reset>::update(std::queue<int> & aqDeceased) {

	if (aqDeceased.empty())
		return;

	//Iterate through all deceased
	while (aqDeceased.empty() == false) {

		int nID = aqDeceased.front();
		aqDeceased.pop();

		AgentBase * pAgent = (*m_pvecAgents)[nID];
		pAgent->setAge(mapLifeTable.cbegin()->first);
		pAgent->resetAdjacentAgentProfiles();

	}
}

