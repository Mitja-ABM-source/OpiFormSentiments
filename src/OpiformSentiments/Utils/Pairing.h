#ifndef OPIFORM_PAIRING_H
#define OPIFORM_PAIRING_H

#include <vector>

namespace opiform {

	class Pairing {
	public:
		typedef std::vector<std::pair<int,int> > Pairs;

		Pairing();
		virtual ~Pairing() = 0;

		virtual void init(const std::vector <class AgentBase * > & avecAgents) = 0;

		virtual bool run(std::vector <class AgentBase * > * apvecAgents, Pairs & avecPairs) = 0;

		enum PairingType {
			Irving,
			Universal
		};

		static Pairing * create(const PairingType & aFunctionType);

	};
}

#endif