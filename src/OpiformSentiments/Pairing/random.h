#ifndef OPIFORM_RANDOM_H
#define OPIFORM_RANDOM_H

#include <map>

#include "../Utils/Pairing.h"

namespace opiform {

	class Random : public Pairing {

	public:
		Random();
		virtual ~Random();

		virtual void init(const std::vector <class AgentBase * > & avecAgents);

		virtual bool run(std::vector <class AgentBase * > * apvecAgents, Pairs & avecPairs);

	private:
		std::vector<struct Agent> setPreferenceList(std::vector <class AgentBase * > * apvecAgents);
		void findfavorites(std::vector<struct Agent> * apvecAgents);
		void reduceTable(std::vector<struct Agent> * apvecAgents, std::map<int, std::vector<int> > & amapReducedTable);
		bool findAndReduceCycles(std::map<int, std::vector<int> > & amapReducedTable);
	};
}

#endif