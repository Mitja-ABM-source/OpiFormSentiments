#include <random>

#include "OpinionUpdating.h"

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

OpinionUpdating::OpinionUpdatingFuncMap OpinionUpdating::s_mapOpinionUpdatingFuncMap = opiform::OpinionUpdating::OpinionUpdatingFuncMap();

//----------------------------------------------------------------------------------------------------------

Opinion::OpinionPosition OpinionUpdating::DWFunction(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & adbAgentMu) {
	
// 	std::normal_distribution<float> dis(0.2f, 0.03f);
//	double dba = dis(gen);
	return (opAgent + adbAgentMu*(opAgentAdj - opAgent));
}

//----------------------------------------------------------------------------------------------------------

Opinion::OpinionPosition OpinionUpdating::kineticFP1Function(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & adbAgentMu) {

	std::normal_distribution<float> dis(0.f,0.1f); //EXCELLENT

	auto dw = [](const Opinion::OpinionPosition & op, const float & aflAlpha) {
		
		double dbT = (1.0 - (1.0 + pow(0.4,0.66)) * op*op);

		return 
			(dbT < 0) ? 0 : sqrt(dbT); //OK
			//(1.0 - abs(op));
			//pow((1.0 - (op*op)),0.2);
	};

	auto pw = [](const Opinion::OpinionPosition & op0, const Opinion::OpinionPosition & op1) {
		return 
			abs(op0);
			//1.0 - op0*op0;
	};

	double dbAlpha = 0.5015;

	//-------------------------------------------------------

	return (opAgent - adbAgentMu*pw(opAgent, opAgentAdj)*(opAgent - opAgentAdj) + dis(gen)*dw(opAgent, dbAlpha));
}

//----------------------------------------------------------------------------------------------------------

Opinion::OpinionPosition OpinionUpdating::kineticFP2Function(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & adbAgentMu) {

	//SS in the center

	std::normal_distribution<float> dis(0.f,0.1f); //EXCELLENT

	auto dw = [](const Opinion::OpinionPosition & op, const float & aflAlpha) {
		
		return (aflAlpha*(1.0 +( op*op)));

	};

	auto kappa = [](const Opinion::OpinionPosition & op, const float & aflKappa) {
		return pow(aflKappa * (1.0 - (op*op)),1.f/3.f);
	};

	//-------------------------------------------------------

	double dbSum = (opAgent + opAgentAdj) / 2.0;
	double dbDif = (opAgent - opAgentAdj) / 2.0;

	return (dbSum + dw(opAgent,0.5)*dbDif);
}

//----------------------------------------------------------------------------------------------------------

Opinion::OpinionPosition OpinionUpdating::kineticFP3Function(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & adbAgentMu) {

	std::normal_distribution<float> dis(0.f,0.1f); //EXCELLENT

	return (opAgent + 0.8 * pow(opAgent - opAgentAdj,2));
}

//----------------------------------------------------------------------------------------------------------
