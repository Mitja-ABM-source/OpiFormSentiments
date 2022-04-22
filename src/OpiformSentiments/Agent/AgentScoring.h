#ifndef OPIFORM_AGENT_SCORING_H
#define OPIFORM_AGENT_SCORING_H

#include <map>
#include <math.h>

namespace opiform {

	class AgentScoring {
	public:
		typedef double(*AgentScoringFunc)(const double &, const double &, const double &);

		enum AgentScoringType {
			RadialBasisKernel = 0
			//Add additional scoring methods, if needed
		};

		typedef std::map<AgentScoringType, AgentScoringFunc> AgentScoringFuncMap;

		static void registerType(const AgentScoringType & anType, const AgentScoringFunc & aFunc) {
			s_mapAgentScoringFuncMap[anType] = aFunc;
		}

		static AgentScoringFunc & getFunction(const AgentScoringType & aFunctionType) {
			return s_mapAgentScoringFuncMap[aFunctionType];
		}

		static const int size() { return s_mapAgentScoringFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

		static inline double RadialBasisKernelFunction(const double & adbAdjPersistence, const double & adbOpinionDistance, const double & adbSentimentNoise) {

			// Ideal Feature Values (Targets)
			double a_0 = 1; // ideal persistence
			double b_0 = 0; // ideal opinion distance
			double c_0 = 1; // ideal sentiment/attitude/feelings

			double a = pow(adbAdjPersistence - a_0, 2);
			double b = pow(adbOpinionDistance - b_0, 2);
			double c = pow(adbSentimentNoise - c_0, 2);
			double d = (exp(-0.5*(a+b+c)));

			return d;
		}

		//------------------------------------------------------------------------------------------------


	private:
		static AgentScoringFuncMap s_mapAgentScoringFuncMap;

	};
}

#endif