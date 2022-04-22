#ifndef OPIFORM_RANKING_MODEL_H
#define OPIFORM_RANKING_MODEL_H

#include "types.h"
#include "../Agent/AgentBase.h"

namespace opiform {

	class RankingModel {
	public:
		typedef void(*RankingModelFunc)(std::vector<AgentCredibility> *, AgentBase *, AgentBase *);

		enum RankingModelType {
			WienerProcess = 0,
			GeneralizedWienerProcess,
			Moscarini,
			Sentiments,
			LinearPersistence
		};

		typedef std::map<RankingModelType, RankingModelFunc> RankingModelFuncMap;

		static void registerType(const RankingModelType & anType, const RankingModelFunc & aFunc) {
			s_mapRankingModelFuncMap[anType] = aFunc;
		}

		static RankingModelFunc & getFunction(const RankingModelType & aFunctionType) {
			return s_mapRankingModelFuncMap[aFunctionType];
		}

		static const int size() { return s_mapRankingModelFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

		static void wienerProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent);

		//------------------------------------------------------------------------------------------------

		static void generalizedWienerProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent);

		//----------------------------------------------------------------------------------------------------------------------

		static void moscariniProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent);

		//----------------------------------------------------------------------------------------------------------------------

		static void sentimentsProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent);

		//----------------------------------------------------------------------------------------------------------------------

		static void persistenceProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent);

		//----------------------------------------------------------------------------------------------------------------------

	private:
		static RankingModelFuncMap s_mapRankingModelFuncMap;

	};
}

#endif