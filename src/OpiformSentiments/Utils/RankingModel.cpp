#include <random>

#include "RankingModel.h"
#include "../Agent/AgentBase.h"

using namespace opiform;

namespace {
	std::random_device rd;

	std::mt19937 gen(
#if RANDOM_SEED
		rd()		
#else
		10
#endif //RANDOM_SEED
		);

}

RankingModel::RankingModelFuncMap RankingModel::s_mapRankingModelFuncMap = opiform::RankingModel::RankingModelFuncMap();

//----------------------------------------------------------------------------------------------------------

void RankingModel::wienerProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent) {
	std::normal_distribution<double> dis(0.0,1.0);
	const float flsqrtT = sqrt(1.f/365.f);
	for (auto & el : (*apAgentData)) {
		el.second += (dis(gen)*flsqrtT); //Wiener process
	}
}

//----------------------------------------------------------------------------------------------------------

void RankingModel::generalizedWienerProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent) {
	std::normal_distribution<double> dis(0.0,1.0);
	const float flsqrtT = sqrt(1.f/365.f);
	for (auto & el : (*apAgentData)) {
		el.second += (0.5 * (1.f/365.f) + 0.3*(dis(gen)*flsqrtT)); //generalized Wiener process
	}
}

//----------------------------------------------------------------------------------------------------------

void RankingModel::moscariniProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent) {

	float flMuH, flMuL, flSigma, flDXt, flPt, flDt = 1.f/365.f;

	float s = (flMuH - flMuL) / flSigma;
	float dZt = (1.0 / flSigma) * (flDXt - flPt*flMuH*flDt - (1.0 - flPt)*flMuL*flDt);

	auto dpt = [&]() {
		float pt = pt * (1.f - pt) * s * dZt;
		return pt;
	};

}

void RankingModel::sentimentsProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent) {
	
	std::uniform_real_distribution<double> disSentiments(0.0, 0.1);
	apAgent->updateAdjAgentSentiments(disSentiments(gen), apAdjAgent->getID());
}

void RankingModel::persistenceProcessFunction(std::vector<AgentCredibility> * apAgentData, AgentBase * apAgent, AgentBase * apAdjAgent) {
	
	if (apAgent == nullptr)
		return;

	Opinion::OpinionTopic t = Opinion::getTopic(); // TODO: implement getTopicPosition() - it is forced to 1 for now

	AdjacentAgentProfile * pMapAdj = apAgent->getAdjAgentMap();
	std::map<int, std::deque < double > > * pMap = &(*pMapAdj)[(Opinion::OpinionTopic)t];
	std::deque < double > * pdeqTemp = &(*pMap)[apAdjAgent->getID()];

	//Lacks full information
	if (pdeqTemp->size() < 3) {
		return;
	}

	else {
		double dbFirst = (*pdeqTemp)[0];
		double dbBeforeLast = (*pdeqTemp)[1];
		double dbLast = (*pdeqTemp)[2];

		double dbRes = 1 - 0.5 * (apAgent->getKappa() * abs(dbBeforeLast - dbFirst) + (1.0 - apAgent->getKappa())*abs(dbLast - dbBeforeLast));
		apAgent->updateAdjAgentPersistenceScore(dbRes, apAdjAgent->getID());
	}
}