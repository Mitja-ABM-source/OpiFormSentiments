#ifndef OPIFORM_OPINION_UPDATING_H
#define OPIFORM_OPINION_UPDATING_H

#include "types.h"

#include "Opinion.h"

namespace opiform {

	class OpinionUpdating {
	public:
		typedef Opinion::OpinionPosition (*OpinionUpdatingFunc)(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & opAgentTheta);

		enum OpinionUpdatingType {
			kineticFP1 = 0,
			kineticFP2,		//Boudin-Salvani
			kineticFP3,		
			DW				//The Deffuant-Weisbuch model
		};

		typedef std::map<OpinionUpdatingType, OpinionUpdatingFunc> OpinionUpdatingFuncMap;

		static void registerType(const OpinionUpdatingType & anType, const OpinionUpdatingFunc & aFunc) {
			s_mapOpinionUpdatingFuncMap[anType] = aFunc;
		}

		static OpinionUpdatingFunc & getFunction(const OpinionUpdatingType & aFunctionType) {
			return s_mapOpinionUpdatingFuncMap[aFunctionType];
		}

		static const int size() { return s_mapOpinionUpdatingFuncMap.size(); };

		//------------------------------------------------------------------------------------------------

		static Opinion::OpinionPosition DWFunction(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & opAgentTheta);

		//------------------------------------------------------------------------------------------------

		static Opinion::OpinionPosition kineticFP1Function(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & opAgentTheta);

		//------------------------------------------------------------------------------------------------

		static Opinion::OpinionPosition kineticFP2Function(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & opAgentTheta);

		//----------------------------------------------------------------------------------------------------------------------

		static Opinion::OpinionPosition kineticFP3Function(const Opinion::OpinionPosition & opAgent, const Opinion::OpinionPosition & opAgentAdj, const double & opAgentTheta);

		//----------------------------------------------------------------------------------------------------------------------

	private:
		static OpinionUpdatingFuncMap s_mapOpinionUpdatingFuncMap;

	};
}

#endif