#ifndef OPIFORM_AGENT_H
#define OPIFORM_AGENT_H

#include <vector>

namespace opiform {

	struct Agent {
	public:
		Agent(){}
		Agent(int anId,const std::vector<int /*Agents' IDs*/> & avecPreferences);

		int getID() const {return m_nId;}

		const int getCurrentBest() const {
			return m_nCurrentBest;
		}

		inline int getNextPosition() {return ++m_nCurrentPosition;}

		void setPreferenceList(const std::vector<int /*Agents' IDs*/> & avecPreferences) {m_vecPreferences=avecPreferences;}
		const std::vector<int> & getIDs() const { return m_vecPreferences;}
		void setFavorite(int anID) {
			m_nCurrentBest=anID;
		}

	private:
		int m_nId;
		int m_nCurrentPosition;
		int m_nCurrentBest;
		std::vector<int /*Agents' IDs*/> m_vecPreferences;
	};

}

#endif