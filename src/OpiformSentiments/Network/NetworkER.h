#ifndef NETWORK_ER_H
#define NETWORK_ER_H

/*
	Erdos–Renyi random graph

	Each node in the graph is probabilistically connected to any other node
*/

#include "NetworkAbstract.h"

namespace opiform {

	class NetworkER : public NetworkAbstract {
	public:
		NetworkER(const double & adbConnectionProb = 0.05);
		virtual ~NetworkER();

		virtual NetworkType getNetworkType() const { return NetworkType::ER; };

		virtual bool generateNetwork(std::vector<AgentBase *> * apvecAgents);
		virtual bool step(std::vector<AgentBase *> * apvecAgents);

	private:
		double m_dbConnectionProb;
	};
}

#endif