#ifndef AGENT_ABSTRACT_H
#define AGENT_ABSTRACT_H

#include <ostream>
#include <vector>
#include <map>
#include <deque>

#include "../Utils/types.h"
#include "../Utils/DecisionMaking.h"
#include "../Utils/Opinion.h"
#include "../Agent/AgentScoring.h"

namespace opiform {

	typedef enum {
		Regular = 0,
		Inconsistent,
		Stubborn
	} AgentType;

	typedef std::map < Opinion::OpinionTopic, std::map < int /*AdjAgentID*/, std::deque < double > > > AdjacentAgentProfile;
	typedef std::map < int /*AdjAgentID*/, std::tuple < int /*MeetingNumber*/, int /*OpinionChangeNumber*/, Opinion::OpinionPosition /*OpinionDifference*/ > > AdjAgentMeetingData;

	class AgentBase {
	public:
		AgentBase(
			const AgentType & aType,
			unsigned int aunAge,
			const double & adbMu = 0.2,
			const double & adbBoundedConfidenceKappa = 0.5,
			const double & adbBoundedConfidenceGamma = 0.5,
			const double & adbBoundedConfidenceTheta = 0.5,
			const Opinion::mapOpinionTopicPosition & amapBelieves = Opinion::mapOpinionTopicPosition(),
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread,
			const AgentScoring::AgentScoringType & aASType = AgentScoring::AgentScoringType::RadialBasisKernel);
		AgentBase(const AgentBase & rhs);
		virtual ~AgentBase() {}

		AgentBase & operator=(const AgentBase & rhs);

		static void resetCounter() { s_nCounter = 0; }
		
		inline void resetAdjacentAgentProfiles() { std::swap(m_mapAdjacentAgentProfiles,opiform::AdjacentAgentProfile()); }

		inline void resetMeetingNumbers() { std::swap(m_mapAdjAgentMeetings, opiform::AdjAgentMeetingData()); }

	private:
		static int s_nCounter;

	protected:
		unsigned int m_unAge;
		int m_nID;
		AgentType m_Type;
		DecisionMaking::DecisionMakingType m_DecisionMakingType;
		AgentScoring::AgentScoringType m_AgentScoringType;

		std::vector<const AgentBase*> m_vecNeighborhood;

		double m_dbMu;
		double m_dbBoundedConfidenceKappa;
		double m_dbBoundedConfidenceGamma;
		double m_dbBoundedConfidenceTheta;

		AdjacentAgentProfile m_mapAdjacentAgentProfiles;
		AdjAgentMeetingData m_mapAdjAgentMeetings;
		std::vector<AgentCredibility> m_vecAdjAgentCredibility;
		std::vector<AgentCredibility> m_vecAdjAgentPreferenceScore;
		std::vector<AgentCredibility> m_vecAdjAgentPersistenceScore;
		std::vector<AgentCredibility> m_vecAdjAgentOpinionDifference;
		std::vector<AgentCredibility> m_vecAdjAgentSentiments;
	
	public:
		const AgentType & getType() const { return m_Type; }
		const DecisionMaking::DecisionMakingType & getDecisionMakingType() const { return m_DecisionMakingType; }
		int getID() const { return m_nID; }

		int getNeighborhoodSize() const { return m_vecNeighborhood.size(); }
		const AgentBase * getNeighbor(int anID);

		bool isMember(const AgentBase * apAgent);

		virtual void updateAdjAgentProfile(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent);

		void updateAdjAgentMeetingNumber(const AgentBase * apAdjAgent, bool bMeeting, const Opinion::OpinionPosition & aOpinionDifference);

		AdjacentAgentProfile * getAdjAgentMap() { AdjacentAgentProfile * ptr = &m_mapAdjacentAgentProfiles;
														return ptr;
													  }
		std::vector<AgentCredibility> * getAdjAgentCredibility() { return &m_vecAdjAgentCredibility; }
		void updateCredibility(const std::vector<AgentCredibility> & avecAdjAgentCredibility);

		std::vector<AgentCredibility> * getAdjAgentPreferenceScore() { return &m_vecAdjAgentPreferenceScore; }
		void updateAdjAgentPreferenceScore(const double & adbScoreAdjacent, const int & anAdjID);

		std::vector<AgentCredibility> * getAdjAgentPersistenceScore() { return &m_vecAdjAgentPersistenceScore; }
		void updateAdjAgentPersistenceScore(const double & adbPersistenceAdjacent, const int & anAdjID);

		std::vector<AgentCredibility> * getAdjAgentSentiments() { return &m_vecAdjAgentSentiments; }
		void updateAdjAgentSentiments(const double & adbSentimentAdjacent, const int & anAdjID);

		std::vector<AgentCredibility> * getAdjAgentOpinionDifference() { return &m_vecAdjAgentOpinionDifference; }
		void updateAdjAgentOpinionDifference(const double & adbOpinionDifferenceAdj, const int & anAdjID);

		bool isAgentCredible(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const double & adbKappa, const double & adbGamma);

		virtual AgentBase * selectInteractingAgent(const Opinion::OpinionTopic & aTopic);
		virtual bool shouldUpdate(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent, const double & adbTheta);

		void addAdjacentAgent(const AgentBase * apAgent);
		void replaceAgent(const AgentBase * apAgentOld, const AgentBase * apAgentNew);
		void removeAgent(const AgentBase * apAgentOld);

		void setAge(const unsigned int & aunAge) { m_unAge = aunAge; }
		int getAge() const { return m_unAge; }
		double getKappa() const { return m_dbBoundedConfidenceKappa; }
		double getGamma() const { return m_dbBoundedConfidenceGamma; }
		double getTheta() const { return m_dbBoundedConfidenceTheta; }

		Opinion::mapOpinionTopicPosition m_mapBelieves; //Position on particular topic
		void setOpinionTopicPosition(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aPosition) {
			m_mapBelieves[aTopic] = aPosition;
		}

		Opinion::mapOpinionTopicPosition * getBelieves() { return &m_mapBelieves; }
		void setBelieves(const Opinion::mapOpinionTopicPosition & amapBelieves);
		inline Opinion::OpinionPosition getTopicPosition(const Opinion::OpinionTopic & aTopic) { return m_mapBelieves[aTopic]; };

		AdjAgentMeetingData * getMeetings() { return &m_mapAdjAgentMeetings; }

		inline double getMu() const { return m_dbMu; }

		friend std::ostream& operator<<(std::ostream& os, const AgentBase & aAgent) {

			os << aAgent.getAge() << '\t';
			const Opinion::mapOpinionTopicPosition * pmapOTP = ((AgentBase *)&aAgent)->getBelieves();

			Opinion::mapOpinionTopicPosition::const_iterator cit = pmapOTP->cbegin(), citEnd = --pmapOTP->cend();
			for (cit; cit != citEnd; ++cit) {
				os << cit->second << '\t';
			}
			os << citEnd->second;

			return os;
		};

	};

}

#endif