#ifndef OPIFORM_PROCESS_DYNAMICS_BASE_H
#define OPIFORM_PROCESS_DYNAMICS_BASE_H

#include <vector>
#include <queue>

#include "../Utils/constants.h"

namespace opiform {

	enum ProcessDynamicsType {
		Continue = 0,
		Reset,
		OLG
	};

	class ProcessDynamicsAbstract {
	protected:
		ProcessDynamicsAbstract(std::vector <class AgentBase * > * apvecAgents);
		virtual ~ProcessDynamicsAbstract() = 0;

		std::vector <class AgentBase * > * m_pvecAgents;
	};


	template < ProcessDynamicsType >
	class ProcessDynamics : public ProcessDynamicsAbstract {
	}; 


	template <>
	class ProcessDynamics<ProcessDynamicsType::Continue> : public ProcessDynamicsAbstract {	
	public:
		ProcessDynamics(std::vector <class AgentBase * > * apvecAgents);
		virtual ~ProcessDynamics();

		void update(std::queue<int> & aqDeceased);
	};


	template <>
	class ProcessDynamics<ProcessDynamicsType::OLG> : public ProcessDynamicsAbstract {	
	public:
		ProcessDynamics(std::vector <class AgentBase * > * apvecAgents) : ProcessDynamicsAbstract(apvecAgents) {}
		virtual ~ProcessDynamics() {}

		void update(std::queue<int> & aqDeceased);
	};


	template <>
	class ProcessDynamics<ProcessDynamicsType::Reset> : public ProcessDynamicsAbstract {	
	public:
		ProcessDynamics(std::vector <class AgentBase * > * apvecAgents) : ProcessDynamicsAbstract(apvecAgents) {}
		virtual ~ProcessDynamics() {};

		void update(std::queue<int> & aqDeceased);
	};

}

#endif