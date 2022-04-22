#include <random>
#include <fstream>
#include <sstream>
#include <queue>

#include "../Utils/PoliticalView.h"

#include "../Utils/Writer.h"
#include "../Agent/AgentBase.h"
#include "../Agent/AgentStubborn.h"
#include "../Agent/AgentInconsistent.h"
#include "../Agent/AgentScoring.h"

#include "../Utils/RankingModel.h"
#include "../Utils/OpinionUpdating.h"
#include "../Utils/Pairing.h"
#include "../Utils/Stats.h"

#include "../Network/NetworkAbstract.h"
#include "ProcessDynamicsBase.h"

#include "../Utils/constants.h"

#include "Game.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;

	std::mt19937 gen(
#if RANDOM_SEED
		rd()		
#else
		10
#endif //RANDOM_SEED
		);
}

Game::Game(const std::string & astrFileName) : m_strFileName(astrFileName) {
}

//--------------------------------------------------------------------------------------

Game::~Game() {
	std::vector<AgentBase*>::iterator it = m_vecAgents.begin();
	while (it != m_vecAgents.end()) {
		delete *it;
		(*it) = NULL;
		++it;
	}
}

//--------------------------------------------------------------------------------------

void Game::registerStatics() {
	// Differences in opinions 
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::LinearSpread, DecisionMaking::linearSpreadFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::Tanh, DecisionMaking::tanhFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::Logistic, DecisionMaking::logisticFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::InvExp, DecisionMaking::invExpFunction);
	DecisionMaking::registerType(DecisionMaking::DecisionMakingType::FermiDirac, DecisionMaking::fermiDiracFunction);

	// Scoring Methods of Agents
	AgentScoring::registerType(AgentScoring::AgentScoringType::RadialBasisKernel, AgentScoring::RadialBasisKernelFunction);

	// Opinion update methods
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::DW, OpinionUpdating::DWFunction);
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::kineticFP1, OpinionUpdating::kineticFP1Function);
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::kineticFP2, OpinionUpdating::kineticFP2Function);
	OpinionUpdating::registerType(OpinionUpdating::OpinionUpdatingType::kineticFP3, OpinionUpdating::kineticFP3Function);

	// Models for updating agents scores
	RankingModel::registerType(RankingModel::RankingModelType::WienerProcess, RankingModel::wienerProcessFunction);
	RankingModel::registerType(RankingModel::RankingModelType::GeneralizedWienerProcess, RankingModel::generalizedWienerProcessFunction);
	RankingModel::registerType(RankingModel::RankingModelType::Moscarini, RankingModel::moscariniProcessFunction);
	RankingModel::registerType(RankingModel::RankingModelType::Sentiments, RankingModel::sentimentsProcessFunction);
	RankingModel::registerType(RankingModel::RankingModelType::LinearPersistence, RankingModel::persistenceProcessFunction);

	Opinion::registerType(Opinion::Euclidian, Opinion::euclidianDist);
	Opinion::registerType(Opinion::EuclidianFermi, Opinion::euclidianFermiDist);
	Opinion::registerType(Opinion::Jensen, Opinion::jensenDist);

}

//--------------------------------------------------------------------------------------

bool Game::init(int anSize, double adbMu, double adbKappaMean, double adbGammaMean, double adbThetaMean) {

	gen.seed(10);

	AgentBase::resetCounter();

	//Init agents
	initAgents(anSize, adbMu, adbKappaMean, adbGammaMean, adbThetaMean);

	Utils::writeAgents(m_vecAgents, Utils::getFolder() + "agents_init.txt");

	return true;
}

//--------------------------------------------------------------------------------------

bool Game::initNetwork(const NetworkType & aNetworkType, NetworkAbstractParams * apNetworkParams) {

	NetworkAbstract * pNet = NetworkAbstract::findAndCreateNetwork(aNetworkType, apNetworkParams);

	if (pNet == nullptr)
		return false;

	bool bRes = pNet->generateNetwork(&m_vecAgents);
	if (bRes == false)
		return false;

	Utils::writeNetwork(m_vecAgents, Utils::getFolder() + "Network.txt");
	pNet->setOpiformModel(apNetworkParams->m_OpinionFormationModelType);

	bRes = NetworkAbstract::isConnected(&m_vecAgents);

	return true;
}

//--------------------------------------------------------------------------------------

int Game::getAgents() const {
	return m_vecAgents.size();
}

//--------------------------------------------------------------------------------------

void Game::runGame(const time_t & aTime) {

	std::uniform_real_distribution<double> disPrefScore(0, 1);

	// POINTERS & OBJECTS before the opinion exchange starts:
	// * pairing type
	opiform::Pairing * pPairing = opiform::Pairing::create(opiform::Pairing::Irving);
	// in case one needs Irving pairing, switch Universal to Irving (roommate algorithm)

	// * persistence score function type
	opiform::RankingModel::RankingModelFunc * pPersistenceProcess = 
		&opiform::RankingModel::getFunction(opiform::RankingModel::RankingModelType::LinearPersistence);

	// * sentiments type
	opiform::RankingModel::RankingModelFunc * pSentiments =
		&opiform::RankingModel::getFunction(opiform::RankingModel::RankingModelType::Sentiments);

	// * opinion update rule type
	opiform::OpinionUpdating::OpinionUpdatingFunc * pOpinionUpdating = 
		&opiform::OpinionUpdating::getFunction(opiform::OpinionUpdating::DW);

	// * preference scoring rule
	opiform::AgentScoring::AgentScoringFunc * pScoringModelFunc =
		&opiform::AgentScoring::getFunction(opiform::AgentScoring::AgentScoringType::RadialBasisKernel);

	// * a type of process dynamics
//	const ProcessDynamicsType ndt = ProcessDynamicsType::Reset;
	ProcessDynamics<ProcessDynamicsType::Reset> processDynamics(&m_vecAgents);

	for (int nTime=0;nTime<aTime;++nTime) {

		Opinion::OpinionTopic t = Opinion::getTopic();
		// std::queue<int> qDeceased = listDeceased();
		// processDynamics.update(qDeceased);

		if (nTime < 1) {
			// set up randomly distributed initial preference scores
			// Initialize Persistence Scores();
			for (auto & p : m_vecAgents) {
				for (auto it = 0; it != p->getNeighborhoodSize(); ++it) {
					p->updateAdjAgentPreferenceScore(disPrefScore(gen), p->getNeighbor(it)->getID());
					p->updateAdjAgentPersistenceScore(disPrefScore(gen), p->getNeighbor(it)->getID());
					p->updateAdjAgentSentiments(disPrefScore(gen), p->getNeighbor(it)->getID());
				}
			}
		}

		//Generate pairs
		// Universal::Random gives random pairs drawn from the whole society
		// Irwing: performs roommate algorithm
		Pairing::Pairs vecPairs;
		bool bRes = pPairing->run(&m_vecAgents, vecPairs);

		//Go through all pairs
		for (auto & p : vecPairs) {
			AgentBase * pAgent = m_vecAgents[p.first];
			AgentBase * pAgentAdjacent = m_vecAgents[p.second];

			//Store opinions before a change
			Opinion::OpinionPosition opinionAgent = pAgent->getTopicPosition(t);
			Opinion::OpinionPosition opinionAdjacent = pAgentAdjacent->getTopicPosition(t);

			double dbKappaTemp = pAgent->getKappa(); // persistence score parameter kappa
			double dbGammaTemp = pAgent->getGamma(); // persistence score ceiling (agent's)
			double dbThetaTemp = pAgent->getTheta(); // opinion threshold parameter theta (agent-specific)
			double dbMuTemp = pAgent->getMu(); // bounded confidence parameter lambda (agent-specific)
			double dbKappaTempAdj = pAgentAdjacent->getKappa(); // persistence score parameter kappa
			double dbGammaTempAdj = pAgentAdjacent->getGamma(); // persistence score ceiling (agent's)
			double dbThetaTempAdj = pAgentAdjacent->getTheta(); // opinion threshold parameter theta (adjacent agent-specific)
			double dbMuTempAdj = pAgentAdjacent->getMu(); // bounded confidence parameter lambda (adjacent agent-specific)
			
			// METHODS:
			//			1.) PERSISTENCE (MORE METHODS CAN EXIST - IMPLEMENT AND REGISTER AVAILABLE FUNCTIONS IN MAP
			//				RankingModel::persistenceProcessFunction: Debug the Method - SEEMS OK
			//			2.)	SENTIMENTS (RANDOM ABSOLUTE NOISE AROUND 0; ...)
			//			3.) CALL APPROPRIATE OPINION DIFFERENCE METHOD FROM THE LIST (LINEAR, ETC.)

			std::vector<AgentCredibility> * vecAdjAgentPersistenceScore = pAgent->getAdjAgentPersistenceScore();
			std::vector<AgentCredibility> * vecAgentPersistenceScore = pAgentAdjacent->getAdjAgentPersistenceScore();
			std::vector<AgentCredibility> * vecAdjAgentSentiments = pAgent->getAdjAgentSentiments();
			std::vector<AgentCredibility> * vecAgentSentiments = pAgentAdjacent->getAdjAgentSentiments();
			opiform::DecisionMaking::DecisionMakingFunc * pOpinionDifferenceFunc =
				&opiform::DecisionMaking::getFunction(pAgent->getDecisionMakingType());

			bool bMeeting = false;
			if (pAgent->shouldUpdate(t, opinionAdjacent, dbThetaTemp)) { // + update Agent's containers with meeting data
				Opinion::OpinionPosition op = (*pOpinionUpdating)(opinionAgent, opinionAdjacent, dbMuTemp);
				pAgent->setOpinionTopicPosition(t, op);
				pAgent->updateAdjAgentMeetingNumber(pAgentAdjacent, bMeeting = true, opinionAgent - op);
			}
			else { pAgent->updateAdjAgentMeetingNumber(pAgentAdjacent, bMeeting = false, 0); } // 0: opinion stays the same

			// PREFERENCE SCORING AFTER/DURING THE MEETING
			//	1.) Persistence
			(*pPersistenceProcess)(vecAdjAgentPersistenceScore, pAgent, pAgentAdjacent);
			// 2. Sentiments
			// * obtain sentiment effects after encounter of agent and adjacent agent
			(*pSentiments)(vecAdjAgentSentiments, pAgent, pAgentAdjacent);
			// 3.) Obtain opinion differences and place them in the appropriate container vecAdjAgentDifference
			// * obtain differences in opinions
			// update opinion difference vector for agent and its adjAgent
			pAgent->updateAdjAgentOpinionDifference((*pOpinionDifferenceFunc)(opinionAgent, opinionAdjacent), pAgentAdjacent->getID());

			if (pAgentAdjacent->shouldUpdate(t, opinionAgent, dbThetaTempAdj)) {// + update AdjAgent's containers with meeting stats
				Opinion::OpinionPosition op = (*pOpinionUpdating)(opinionAdjacent, opinionAgent, dbMuTempAdj);
				pAgentAdjacent->setOpinionTopicPosition(t, op);
				pAgentAdjacent->updateAdjAgentMeetingNumber(pAgent, bMeeting = true, opinionAdjacent - op);
			}
			else { pAgentAdjacent->updateAdjAgentMeetingNumber(pAgent, bMeeting = false, 0); } // 0: opinion stays the same
		
			// PREFERENCE SCORING AFTER/DURING THE MEETING
			//	1.) Persistence
			(*pPersistenceProcess)(vecAgentPersistenceScore, pAgentAdjacent, pAgent);
			// 2. Sentiments
			// * obtain sentiment effects after encounter of agent and adjacent agent
			(*pSentiments)(vecAgentSentiments, pAgentAdjacent, pAgent);
			// 3.) Obtain opinion differences and place them in the appropriate container vecAdjAgentDifference
			// * obtain differences in opinions
			// update opinion difference vector for agent and its adjAgent
			pAgentAdjacent->updateAdjAgentOpinionDifference((*pOpinionDifferenceFunc)(opinionAdjacent, opinionAgent), pAgent->getID());

			// PERFORMS PREFERENCE SCORING BEFORE NEXT PAIRING
			// Initialize (empty) inputs for Scoring

			double dbPersistence, dbPersistenceAdj = 0;
			double dbDifference = 0;
			double dbSentiments, dbSentimentsAdj = 0;

			// Inputs to Scores
			int nI = 0;
			for (const auto &pair : *vecAdjAgentPersistenceScore) {
				if (pair.first == pAgentAdjacent->getID()) {
					const auto *vecPersistence = &(*vecAdjAgentPersistenceScore)[nI];
					dbPersistence = vecPersistence->second;
					break;
				}
				nI++;
			}

			int nII = 0;
			for (const auto &pair : *vecAgentPersistenceScore) {
				if (pair.first == pAgent->getID()) {
					const auto *vecPersistence = &(*vecAgentPersistenceScore)[nII];
					dbPersistenceAdj = vecPersistence->second;
					break;
				}
				nII++;
			}

			int nJ = 0;
			for (const auto &pair : *vecAdjAgentSentiments) {
				if (pair.first == pAgentAdjacent->getID()) {
					const auto *vecSentiments = &(*vecAdjAgentSentiments)[nJ];
					dbSentiments = vecSentiments->second;
					break;
				}
				nJ++;
			}

			int nJJ = 0;
			for (const auto &pair : *vecAgentSentiments) {
				if (pair.first == pAgent->getID()) {
					const auto *vecSentimentsAdj = &(*vecAgentSentiments)[nJJ];
					dbSentimentsAdj = vecSentimentsAdj->second;
					break;
				}
				nJJ++;
			}

			dbDifference = (*pOpinionDifferenceFunc)(opinionAdjacent, opinionAgent);

			// Obtain and update preference scores for agent and the adjacent contact
			double agentScore = 0;
			double agentScoreAdj = 0;
			agentScore = (*pScoringModelFunc)(dbPersistence, dbDifference, dbSentiments);
			agentScoreAdj = (*pScoringModelFunc)(dbPersistenceAdj, dbDifference, dbSentimentsAdj);

			pAgent->updateAdjAgentPreferenceScore(agentScore, pAgentAdjacent->getID());
			pAgentAdjacent->updateAdjAgentPreferenceScore(agentScoreAdj, pAgent->getID());

		}

		// Update age
		//for (const auto & agent : m_vecAgents) {
		//	agent->setAge(agent->getAge() + 1);
		//}
		
		// Write
		if (nTime % 100 == 0) {
			string strName = "results_SB_" + to_string(nTime) + ".txt";
			Utils::writeAgents(m_vecAgents, Utils::getFolder() + strName);
		}
	}
	// Comment-out for registering last iteration results - USE FOR DEBUGGING
	// Utils::writeAgents(m_vecAgents, Utils::getFolder() + m_strFileName);
	
	// TODO: Prepare Statistical Analysis and write it out
	//		 USE: writeStats method in Utils class on vector<Stats> data from the Stats class (its Statistics struct)
	std::vector<Statistics> vecStatsOut;
	Stats::analyze(m_vecAgents, vecStatsOut);
	Utils::writeStats(vecStatsOut, Utils::getFolder() + "stats.txt");
}

//---------------------------------------------------------------------------------------------------------------

void Game::initAgents(int anSize, const double & adbMuMean, const double & adbKappaMean, const double & adbGammaMean, const double & adbThetaMean) {
	m_vecAgents.reserve(anSize + 1);

	int nTopics = Opinion::getNumTopics();

	// std::uniform_real_distribution<double> dis(-1.0,1.0);
	std::normal_distribution<double> dis(0, 0.5);
	std::uniform_int_distribution<int> disAge(mapLifeTable.cbegin()->first, 80 * 12);
	std::normal_distribution<double> disKappa(adbKappaMean, 0.1*adbKappaMean);
	std::normal_distribution<double> disGamma(adbGammaMean, 0.1*adbGammaMean);
	std::normal_distribution<double> disTheta(adbThetaMean, 0.1*adbThetaMean);
	std::normal_distribution<double> disMu(adbMuMean, 0.1*adbMuMean);

	for (int nI = 0; nI < anSize; ++nI) {
		//Set initial topic positions
		Opinion::mapOpinionTopicPosition mapBelieves;

		for (int nT = 0; nT < nTopics; ++nT) {
			mapBelieves[(Opinion::OpinionTopic)nT] = (Opinion::OpinionPosition)dis(gen);
		}

		double dbT = mapBelieves[(Opinion::OpinionTopic)0];
		
		// initial age of each agent
		int nAge = disAge(gen);
		// bounded confidence mu (lambda), persistence threshold gamma, and opinion threshold theta of each agent
		double dbPersistenceKappa = disKappa(gen);
		double dbPersistenceThresholdGamma = disGamma(gen);
		double dbOpinionThresholdTheta = disTheta(gen);
		double dbMu = disMu(gen);

		// create pointer to Agent (objects)
		
		AgentBase * pA = new AgentBase(AgentType::Regular, nAge, dbMu /*mu*/, dbPersistenceKappa /* kappa in opinion dynamics */, dbPersistenceThresholdGamma /* gamma bound on persistence score */, dbOpinionThresholdTheta /* Agent-based Theta in opinion adoption rule */, mapBelieves /*opinoon & topic*/,
			DecisionMaking::DecisionMakingType::LinearSpread, AgentScoring::AgentScoringType::RadialBasisKernel);

		// push back pointer in the member agent list (vector)
		m_vecAgents.push_back(pA);
	}
}

//------------------------------------------------------------------------------------------------------

std::queue<int> Game::listDeceased() {
	//Check deceased
	std::queue<int> qDeceasedIDs;
	int nSize = m_vecAgents.size();
	uniform_real_distribution<double> dis;

	for (int nI=0;nI<nSize; ++nI) {
		AgentBase * pAgent = m_vecAgents[nI];

		//Find appropriate age-group probability from the Table
		int nAge = pAgent->getAge();
		auto it = mapLifeTable.upper_bound(nAge);

		const double * pdbProb = &((--it)->second);

		if (dis(gen) < *pdbProb) {
			qDeceasedIDs.push(nI);
		}
	}

	return qDeceasedIDs;
}

//------------------------------------------------------------------------------------------------------

// void Game::updateRanking() {
	// Updates scores and refreshes (sorts) the new rankings ascendingly

	// Read function from the map
//	const opiform::AgentScoring::AgentScoringFunc * pScoringModelFunc =
//		&opiform::AgentScoring::getFunction(opiform::AgentScoring::AgentScoringType::RadialBasisKernel);
//
//	// iterate through each agent and obtain the preference score by using RBK
//	for (auto * pAgent : m_vecAgents) {
//		int nSize = pAgent->getNeighborhoodSize();
//		std::vector<AgentCredibility> * adbAdjPersistence = pAgent->getAdjAgentPersistenceScore();
//		std::vector<AgentCredibility> * adbAdjDifference = pAgent->getAdjAgentOpinionDifference();
//		std::vector<AgentCredibility> * adbAdjSentiments = pAgent->getAdjAgentSentiments();
//
//		for (int i = 0; i < nSize; i++) {
//			auto * vecDifference = &(*adbAdjDifference)[i];
//			auto * vecPersistence = &(*adbAdjPersistence)[i];
//			auto * vecSentiments = &(*adbAdjSentiments)[i];
//			double agent_score = (*pScoringModelFunc)(vecPersistence->second, vecDifference->second, vecSentiments->second);
//			pAgent->updateAdjAgentPreferenceScore(agent_score, pAgent->getNeighbor(i)->getID());
//		}
//	}
// }