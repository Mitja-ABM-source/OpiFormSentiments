#ifndef NETWORK_SW_H
#define NETWORK_SW_H

#include "NetworkAbstract.h"

namespace opiform {

	class NetworkSW : public NetworkAbstract {
	public:
		NetworkSW(int anNeighbors = 6, const double & adbRewiringProb = 0.05);
		virtual ~NetworkSW();

		virtual NetworkType getNetworkType() const { return NetworkType::SW; };

		virtual bool generateNetwork(std::vector<AgentBase *> * apvecAgents);
		virtual bool step(std::vector<AgentBase *> * apvecAgents);
		virtual bool isConnected(const std::vector<AgentBase*> * apvecAgents) { return true; };

	protected:
		bool generateRegularLattice(std::vector<AgentBase *> * apvecAgents);

		virtual void networkRewiring(std::vector<AgentBase *> * apvecAgents, const double & adbProb);
		int m_nNeighbors;
		double m_dbRewiringProb;
	};
}

#endif