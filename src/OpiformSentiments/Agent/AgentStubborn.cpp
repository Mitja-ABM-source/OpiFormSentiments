#include "AgentStubborn.h"

using namespace opiform;

AgentStubborn::AgentStubborn(unsigned int anAge, const double & adbBoundedConfidenceKappa, const double & adbBoundedConfidenceGamma, const double & adbBoundedConfidenceTheta, const Opinion::mapOpinionTopicPosition & amapBelieves, const DecisionMaking::DecisionMakingType & aDMType) : AgentBase(AgentType::Stubborn, anAge, 0.0, adbBoundedConfidenceKappa, adbBoundedConfidenceGamma, adbBoundedConfidenceTheta, amapBelieves, aDMType) {
}

//------------------------------------------------------------------------------------------

AgentStubborn::AgentStubborn(const AgentStubborn & rhs) : AgentBase(rhs)	{
	this->m_unAge = rhs.m_unAge;
	this->m_nID = rhs.m_nID;
	this->m_dbBoundedConfidenceKappa = rhs.m_dbBoundedConfidenceKappa;
	this->m_dbBoundedConfidenceGamma = rhs.m_dbBoundedConfidenceGamma;
	this->m_dbBoundedConfidenceTheta = rhs.m_dbBoundedConfidenceTheta;
	this->m_Type = rhs.m_Type;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;
}

//------------------------------------------------------------------------------------------

AgentStubborn::~AgentStubborn() {
}

//------------------------------------------------------------------------------------------

AgentStubborn & AgentStubborn::operator=(const AgentStubborn & rhs) {
	// check for self-assignment
	if (&rhs == this)
		return *this;

	this->m_nID = rhs.m_nID;
	this->m_Type = rhs.m_Type;
	this->m_dbBoundedConfidenceKappa = rhs.m_dbBoundedConfidenceKappa;
	this->m_dbBoundedConfidenceGamma = rhs.m_dbBoundedConfidenceGamma;
	this->m_dbBoundedConfidenceTheta = rhs.m_dbBoundedConfidenceTheta;
	this->m_DecisionMakingType = rhs.m_DecisionMakingType;
	this->m_vecNeighborhood = rhs.m_vecNeighborhood;

	return *this;
}

//------------------------------------------------------------------------------------------
