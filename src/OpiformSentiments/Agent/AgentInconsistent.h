#ifndef AGENT_INCONSISTENT_H
#define AGENT_INCONSISTENT_H

#include "AgentBase.h"

/*
	An inconsistent agent randomly changes personal opinion at each occasion
*/

namespace opiform {

	class AgentInconsistent : public AgentBase {
	public:
		AgentInconsistent(
			unsigned int anAge,
			const double & adbMu = 0.2,
			const double & adbBoundedConfidenceKappa = 0.5,
			const double & adbBoundedConfidenceGamma = 0.5,
			const double & adbBoundedConfidenceTheta = 0.5,
			const Opinion::mapOpinionTopicPosition & amapBelieves = Opinion::mapOpinionTopicPosition(),
			const DecisionMaking::DecisionMakingType & aDMType = DecisionMaking::DecisionMakingType::LinearSpread);
		AgentInconsistent(const AgentInconsistent & rhs);

		virtual ~AgentInconsistent();
		AgentInconsistent & operator=(const AgentInconsistent& rhs);

		virtual double getOpinion() const;

		virtual bool shouldUpdate(const Opinion::OpinionTopic & aTopic, const Opinion::OpinionPosition & aOpinionAdjacent);
	};
}

#endif