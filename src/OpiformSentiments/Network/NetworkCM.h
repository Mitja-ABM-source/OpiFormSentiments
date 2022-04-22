#ifndef NETWORK_CM_H
#define NETWORK_CM_H

#include "NetworkAbstract.h"

namespace opiform {

	class NetworkCM : public NetworkAbstract {
	public:
		NetworkCM(int anGroupSize = 10);
		virtual ~NetworkCM();

		virtual NetworkType getNetworkType() const { return NetworkType::CM; };

		virtual bool generateNetwork(std::vector<AgentBase *> * apvecAgents);

		virtual bool step(std::vector<AgentBase *> * apvecAgents);

	private:
		int m_nGroupSize;
	};
}

#endif