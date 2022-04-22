#include <vector>
#include <fstream>
#include <iterator>
#include <map>

#include "../Agent/AgentBase.h"
#include "Stats.h"

#include "Writer.h"

using namespace std;
using namespace opiform;

string Utils::s_strFolder = ".\\";

void Utils::setFolder(const std::string & astrFolder) {
	s_strFolder = astrFolder;
}

std::string Utils::getFolder() {
	return s_strFolder;
}

void Utils::writeAgents(const std::vector<AgentBase*> & aT, const string & astrFile, const char * apchDelim) {
	std::ofstream out;
	out.open(astrFile, std::ios_base::app);

	vector<AgentBase*>::const_iterator cit = aT.cbegin(), citEnd = --aT.cend();

	for (cit; cit != citEnd; ++cit) {
		out << *(*cit) << '\n';
	}
	out << *(*citEnd);
	
	out << endl;
	out.close();
}

//----------------------------------------------------------------------------------------------------------------------

void Utils::writeNetwork(const std::vector<AgentBase*> & avecAgents, const string & astrFile) {
	std::ofstream out;
	out.open(astrFile);

	out << "Agent and agent's connections\n";
	for (int nI = 0; nI < avecAgents.size(); ++nI) {

		AgentBase * pAgent = avecAgents[nI];
		out << pAgent->getID() << "\t";
		for (int nJ = 0; nJ < pAgent->getNeighborhoodSize(); ++nJ) {
			out << pAgent->getNeighbor(nJ)->getID() << "\t";
		}
		out << "\n";
	}

	//Collect number of connections per agent
	out << "Connections per agent\nConnections\n";
	for (int nI = 0; nI < avecAgents.size(); ++nI) {
		AgentBase * pAgent = avecAgents[nI];
		out << pAgent->getNeighborhoodSize() << "\n";
	}

	//Collect number of agents per number of connections
	std::map<int, int> m;
	for (int nI = 0; nI < avecAgents.size(); ++nI) {
		AgentBase* pAgent = avecAgents[nI];
		++m[pAgent->getNeighborhoodSize()];
	}

	out << "\n";
	out << "Agents per connections\nConnections Agents\n";
	std::map<int, int>::const_iterator it = m.cbegin();
	for (it; it != m.cend(); ++it) {
		out << it->first << "\t" << it->second << "\n";
	}

	out.close();

}

//------------------------------------------------------------------------------------

void Utils::writeStats(const std::vector<Statistics> & avecStats, const std::string & astrFile) {
	ofstream out;
	out.open(astrFile, std::ios_base::app);

	if (out.is_open()) {

		int nI = 0;
		for (const Statistics & el : avecStats) {
			out
				<< nI++ << '\t'				//opinion topic ID
				<< el.m_dbMean << '\t'		//mean
				<< el.m_dbVar << '\t'		//variance
				<< el.m_nMeetingsNumber << '\t'
				<< el.m_nOpinionChangeNumber << '\t'
				<< el.m_dbOverallOpinionChange << '\n';
		}
	}

	out.close();
}