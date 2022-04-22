#include <vector>

#include "../Agent/AgentBase.h"
#include "../Pairing/roommate.h"
#include "../Pairing/random.h"

#include "Pairing.h"

using namespace std;
using namespace opiform;

Pairing::Pairing() {
}

Pairing::~Pairing() {
}

Pairing * Pairing::create(const PairingType & aFunctionType) {
	switch (aFunctionType)
	{
	case PairingType::Irving:
		return new Roommate;
		break;
	case PairingType::Universal:
		return new Random;
		break;
	default:
		return nullptr;
	}
}