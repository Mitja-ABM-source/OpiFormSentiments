#include "../Agent/AgentBase.h"
#include "ProcessDynamicsBase.h"

using namespace opiform;

ProcessDynamicsAbstract::ProcessDynamicsAbstract(std::vector <class AgentBase * > * apvecAgents) : m_pvecAgents(apvecAgents) {
}

ProcessDynamicsAbstract::~ProcessDynamicsAbstract()	{
	int nIndy = -1;
}
