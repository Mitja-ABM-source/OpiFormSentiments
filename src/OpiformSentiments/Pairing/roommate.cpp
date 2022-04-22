#include <iostream>
#include <deque>
#include <map>
#include <algorithm>

#include "../Agent/AgentBase.h"

#include "agent.h"
#include "roommate.h"

using namespace std;

opiform::Roommate::Roommate() : Pairing() {
}

opiform::Roommate::~Roommate() {
}

//------------------------------------------------------------------------------

void opiform::Roommate::init(const std::vector <class AgentBase * > & avecAgents) {
}

//------------------------------------------------------------------------------

bool opiform::Roommate::run(std::vector <class AgentBase * > * apvecAgents, Pairs & avecPairs) {

	std::vector<Agent> vecAgents = setPreferenceList(apvecAgents);

	findfavorites(&vecAgents);
	std::map<int, std::vector<int> > mapReducedTable;
	reduceTable(&vecAgents, mapReducedTable);

	bool bRes = findAndReduceCycles(mapReducedTable);

	// Identify pairs
	size_t nSize = vecAgents.size();
	Pairs vecPairs;
	vecPairs.reserve(nSize);
	for (const auto & el : mapReducedTable) {
		if (el.second.size() != 1)
			continue;
		vecPairs.push_back(std::pair<int,int>(el.first,el.second[0]));
	}

	auto removeSymPairs = [](Pairs & avecPairs) {
		map<int, int> hashMap;
		Pairs vecRes;

		for(const auto & el : avecPairs) {
			int first = el.first;
			int second = el.second;

			/* Check if it is symmetric pair */
			if (hashMap[second] == first) {
				vecRes.push_back(pair<int,int>(first, second));
			}
			/* Else make an entry into hash table */
			else {
				hashMap[first] = second;
			}
		}

		return vecRes;
	};

	avecPairs = removeSymPairs(vecPairs);

	if (avecPairs.size() > 0)
		return true;

	return false;
}

//-----------------------------------------------------------------------------------------------------

std::vector<opiform::Agent> opiform::Roommate::setPreferenceList(std::vector <class AgentBase * > * apvecAgents) {
	std::vector<Agent> vecAgents(apvecAgents->size());

#if 0
	int nnSize = apvecAgents->size();
	for (int nn=0;nn<nnSize;++nn) {
		AgentBase * el = (*apvecAgents)[nn];
		int nSize = el->getNeighborhoodSize();
		vector<int> vecs(nSize);
		for(int nI=0;nI<nSize;++nI) {
			vecs[nI]=el->getNeighbor(nI)->getID();
		}
		std::random_shuffle(vecs.begin(),vecs.end());
		vecAgents[nn] = (Agent(el->getID(),vecs));
	}
#else

	int nSize = apvecAgents->size();
	for(int nI = 0; nI<nSize;++nI) {
		AgentBase * el = (*apvecAgents)[nI];
		auto * adjPref = el->getAdjAgentPreferenceScore();
		vector<int> vecs(adjPref->size());
		for(int nJ=0;nJ<adjPref->size();++nJ) {
			auto * vec = &(*adjPref)[nJ];
			vecs[nJ]=vec->first;
		}

		vecAgents[nI] = Agent(el->getID(),vecs);
	}

#endif
	return vecAgents;
}

//-----------------------------------------------------------------------------------------------------

void opiform::Roommate::findfavorites(std::vector<struct Agent> * apvecAgents) {

	auto shouldAccept = [](const Agent & aAgentProposer, const Agent & aAgentProposed) {
		int nCurrent = aAgentProposed.getCurrentBest();
		if (nCurrent == -1) //Currently unmatched => accept
			return true;

		int nProposer = aAgentProposer.getID();

		//Check if already paired
		if (nCurrent == nProposer)
			return true;

		const vector<int> & vecIDs = aAgentProposed.getIDs();
		for(int el : vecIDs) {
			if (el==nCurrent)
				return false;
			else if (el == nProposer)
				return true;
		}

		return false;
	};


	auto getNext = [&](opiform::Agent * pAgent) -> int { 
		int nPos = pAgent->getNextPosition();
		auto * pIds = &pAgent->getIDs();
		if (nPos < pIds->size())
			return (*pIds)[nPos];
		else {
			return -1;
		}
	};

	//Initially all are unmatched
	size_t nSize = apvecAgents->size();
	std::deque<int /*ID*/> deqUnmatchedIds(nSize);
	for (int nI=0;nI<nSize;++nI)
		deqUnmatchedIds[nI] = nI; //Agents are listed with positions and not their IDs

	while (deqUnmatchedIds.empty() == false) {
		int nProposerID = deqUnmatchedIds.front();
		deqUnmatchedIds.pop_front();
		Agent * pAgentProposer = &(*apvecAgents)[nProposerID];

		bool bRes = false;
		//Search for the best possible match => iterate through the priority list
		while (bRes == false) {

			int nProposedID = getNext(pAgentProposer);

			//Check if candidate list is exhausted
			if (nProposedID == -1) {
				bRes = false;
				break;
			}

			//Proposed agent
			Agent * pAgentProposed = &(*apvecAgents)[nProposedID];

			bRes = shouldAccept(*pAgentProposer,*pAgentProposed);

			if (bRes == true) {
				//Proposed agent accepts proposal => previous match becomes unmatched, the new one becomes matched		  
				const int nCurrent = pAgentProposed->getCurrentBest();
				pAgentProposed->setFavorite(pAgentProposer->getID());

				if (nCurrent > -1) {
					//Previous best is rejected
					deqUnmatchedIds.push_front(nCurrent);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------

void opiform::Roommate::reduceTable(std::vector<struct Agent> * apvecAgents, std::map<int, std::vector<int> > & amapReducedTable) {
	//1. remove all those to whom Y prefers X

	for (Agent & el : *apvecAgents) {
		const std::vector<int> * pvecIDs = &el.getIDs();

		//Return position of the prefered agent
		auto pos = (std::find(pvecIDs->cbegin(),pvecIDs->cend(),el.getCurrentBest()) - pvecIDs->cbegin());

		//Candidates that agent himself rejected
		if (pos < pvecIDs->size()) {
			vector<int> vecRedTbl((*pvecIDs).cbegin(),(*pvecIDs).cbegin()+pos+1);
			amapReducedTable[el.getID()] = vecRedTbl;
		}
		else
			amapReducedTable[el.getID()] = vector<int>();
	}

	//2. Remove agents of X to whom X was rejected
	auto vecReducedClone(amapReducedTable);
	int nSize = amapReducedTable.size();
	for (int nI=0;nI<nSize;++nI) {

		opiform::Agent * pAgent = &(*apvecAgents)[nI];

		const std::vector<int> * pvecIDs = &pAgent->getIDs();
		std::vector<int>::const_iterator itStart = pvecIDs->cbegin()+vecReducedClone[nI].size();

		while (itStart != pvecIDs->cend()) {
			std::vector<int> * pvec = &amapReducedTable[*itStart];
			pvec->erase(std::remove(pvec->begin(), pvec->end(), pAgent->getID()), pvec->end());
			++itStart;
		}
	}
}

//------------------------------------------------------------------------------------------------------

bool opiform::Roommate::findAndReduceCycles(std::map<int, std::vector<int> > & amapReducedTable) {

	//https://github.com/jtilly/matchingR/blob/master/src/roommate.cpp
	bool bStable = false;
	while (bStable == false) {
		bStable = true;
		for (auto & el : amapReducedTable) {
			if (el.second.size() > 1) {
				bStable = false;

				std::vector<int> x;
				std::vector<int> index;

				int new_index = el.first;
				int rot_tail = -1;

				while (rot_tail == (index.end() - index.begin() - 1)) {
					int new_x = amapReducedTable[new_index][1];
					new_index = amapReducedTable[new_x].back();

					// Check for a rotation
					rot_tail = find(index.begin(), index.end(), new_index) - index.begin();

					x.push_back(new_x);
					index.push_back(new_index);
				}

				// Delete the rotation
				for (int i = rot_tail + 1; i < index.size(); i++) {
					while(amapReducedTable[x[i]].back() != index[i-1]) {
						// find and erase from the table
						// x[i] needs to be removed from  table[table[x[i]].back()], and
						// table[table[x[i]].back()][x[i]] needs to be removed from
						// table[x[i]].
						int tab_size = amapReducedTable[amapReducedTable[x[i]].back()].size();

						// Remove x[i] from table[table[x[i]].back()]
						// If x[i] is not in table[table[x[i]].back()], then it should remove
						// nothing.
						// This uses an 'erase-remove' idiom from the std library.
						amapReducedTable[amapReducedTable[x[i]].back()].erase(std::remove(amapReducedTable[amapReducedTable[x[i]].back()].begin(),
							amapReducedTable[amapReducedTable[x[i]].back()].end(),
							x[i]),
							amapReducedTable[amapReducedTable[x[i]].back()].end());

						// Check to see if it removed x[i] or not (whether the table's the same size)
						if (tab_size == amapReducedTable[amapReducedTable[x[i]].back()].size()) { return false; }

						// Check to see if there's only one element remaining (if so, no stable matching.)
						if (amapReducedTable[x[i]].size() == 1) { return false; }

						// Remove table[table[x[i]].back()][x[i]] from table[x[i]] (it should be at the end).
						amapReducedTable[x[i]].pop_back();
					}
				}
			}
		}
	}

	return true;
}
