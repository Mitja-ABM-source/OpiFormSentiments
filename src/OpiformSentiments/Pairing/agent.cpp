#include "agent.h"

using namespace opiform;

Agent::Agent(int anId,const std::vector<int /*Agents' IDs*/> & avecPreferences) : 
	m_vecPreferences(avecPreferences), m_nId(anId), m_nCurrentBest(-1), m_nCurrentPosition(-1) {
}

//-------------------------------------------------------------------------------------------------
