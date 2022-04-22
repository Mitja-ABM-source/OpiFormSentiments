#include <random>

#include "Opinion.h"

using namespace std;
using namespace opiform;

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
}


std::map<Opinion::DistanceMeasure, Opinion::Func> Opinion::s_mapDistanceMeasures = std::map<Opinion::DistanceMeasure, Opinion::Func>();

Opinion::Opinion() {
}

//---------------------------------------------------------------------------------------

Opinion::~Opinion() {
}

Opinion::OpinionTopic Opinion::getTopic() {
	int nTopics = Opinion::getNumTopics();
	uniform_int_distribution<> dis(0, nTopics - 1);
	return (Opinion::OpinionTopic)dis(gen);
}