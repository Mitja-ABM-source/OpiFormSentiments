#ifndef OPIFORM_OPINION_FORMATION_MODELS_H
#define OPIFORM_OPINION_FORMATION_MODELS_H

#include <map>
#include <iostream>

#include "../Agent/AgentBase.h"

namespace opiform {

	class OpinionFormationModel {
	public:
		typedef int(*OpinionFormationModelFunc)(AgentBase * apAgent, const Opinion::OpinionTopic & aTopic);

		enum OpinionFormationModelType {
			DW = 0			//The Deffuant-Weisbuch model
			//kineticFP1
		};

		typedef std::map<OpinionFormationModelType, OpinionFormationModelFunc> OpinionFormationModelFuncMap;

		static void registerType(const OpinionFormationModelType & anType, const OpinionFormationModelFunc & aFunc) {
			s_mapOpinionFormationModelFuncMap[anType] = aFunc;
		}

		static OpinionFormationModelFunc & getFunction(const OpinionFormationModelType & aFunctionType) {
			return s_mapOpinionFormationModelFuncMap[aFunctionType];
		}

		static const int size() { return s_mapOpinionFormationModelFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

//		static inline int DWFunction(AgentBase* apAgent, const Opinion::OpinionTopic & aTopic) {
//
//			//Select an interacting agent as to the topic
//			AgentBase * pAgentAdjacent = apAgent->selectInteractingAgent(aTopic);
//
//			if (pAgentAdjacent == nullptr)
//				return 0;
//
//			//Store opinions before a change
//			Opinion::OpinionPosition opinionAgent = apAgent->getTopicPosition(aTopic);
//			Opinion::OpinionPosition opinionAdjacent = pAgentAdjacent->getTopicPosition(aTopic);
//
//			apAgent->updateAdjAgentProfile(pAgentAdjacent, aTopic, opinionAdjacent);
//			pAgentAdjacent->updateAdjAgentProfile(apAgent, aTopic, opinionAgent);
//
//			if (apAgent->shouldUpdate(aTopic, opinionAdjacent)) {
//				Opinion::OpinionPosition op = opinionAgent + apAgent->getMu()*(opinionAdjacent - opinionAgent);
//				apAgent->setOpinionTopicPosition(aTopic, op);
//			}
//
//
//#if 1
//			if (pAgentAdjacent->shouldUpdate(aTopic, opinionAgent)) {
//				Opinion::OpinionPosition op = opinionAdjacent + pAgentAdjacent->getMu()*(opinionAgent - opinionAdjacent);
//				pAgentAdjacent->setOpinionTopicPosition(aTopic, op);
//			}
//#endif
//			return 1;
//		}

		//------------------------------------------------------------------------------------------------


	private:
		static OpinionFormationModelFuncMap s_mapOpinionFormationModelFuncMap;

	};
}

#endif