#ifndef NETWORK_BA_H
#define NETWORK_BA_H

/*
	Undirected Barabasi-Albert random scale-free graph

*/

#include "NetworkAbstract.h"

namespace opiform {


	class NetworkBA : public NetworkAbstract {
	public:
		NetworkBA(int anConnectedNodes = 10);
		virtual ~NetworkBA();

		virtual NetworkType getNetworkType() const { return NetworkType::BA; };

		virtual bool generateNetwork(std::vector<AgentBase*> * apvecAgents);

		virtual bool step(std::vector<AgentBase *> * apvecAgents);

	private:
		int m_nConnectedNodes;
	};
}

#endif