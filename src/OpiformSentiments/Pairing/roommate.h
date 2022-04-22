#ifndef OPIFORM_ROOMMATE_H
#define OPIFORM_ROOMMATE_H

#include <map>

#include "../Utils/Pairing.h"

namespace opiform {

	class Roommate : public Pairing {

	public:
		Roommate();
		virtual ~Roommate();

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