#ifndef NETWORK_SW_AGE_H
#define NETWORK_SW_AGE_H

#include "NetworkSW.h"

namespace opiform {

	class NetworkSWAge : public NetworkSW {
	public:
		NetworkSWAge(int anNeighbors = 6, const double & adbRewiringProb = 0.05);
		virtual ~NetworkSWAge();

		virtual NetworkType getNetworkType() const { return NetworkType::SWAge; };

		virtual bool generateNetwork(std::vector<AgentBase *> * apvecAgents);
		virtual bool step(std::vector<AgentBase *> * apvecAgents);
		virtual bool isConnected(const std::vector<AgentBase*> * apvecAgents) { return true; };


	private:
		void networkRewiring(std::vector<AgentBase *> * apvecAgents, const double & adbProb);
	};
}

#endif