#ifndef AGENT_STUBBORN_H
#define AGENT_STUBBORN_H

#include "AgentBase.h"

namespace opiform {

	class AgentStubborn : public AgentBase {
	public:
		AgentStubborn(
			unsigned int anAge,
			const double & adbBoundedConfidenceKappa = 0.5,
			const double & adbBoundedConfidenceGamma = 0.5,
			const double & adbBoundedConfidenceTheta = 0.5,
			const Opinion::mapOpinionTopicPosition & amapBelieves = Opinion::mapOpinionTopicPosition(),
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread);
		AgentStubborn(const AgentStubborn & rhs);

		virtual ~AgentStubborn();
		AgentStubborn & operator=(const AgentStubborn & rhs);

		virtual void updateAdjAgentProfile(const AgentBase * apAgent, const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent) {
			/*Do nothing*/
		};

		virtual AgentBase * selectInteractingAgent(const Opinion::OpinionTopic & aTopic) {
			return nullptr;
		}

		virtual bool shouldUpdate(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent) {
			return false;
		};
	};
}

#endif