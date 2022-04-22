#include <memory>
#include <queue>
#include <random>

#include "../Agent/AgentBase.h"

#include "NetworkFC.h"
#include "NetworkBA.h"
#include "NetworkER.h"
#include "NetworkSW.h"
#include "NetworkCM.h"
#include "NetworkSWAge.h"

#include "NetworkAbstract.h"

using namespace opiform;
using namespace std;

namespace {
	std::random_device rd;

	std::mt19937 gen(
		rd()
//		10
		);
}


NetworkAbstract::~NetworkAbstract()	{
	int nIndy = -1;
}

NetworkAbstract * NetworkAbstract::findAndCreateNetwork(const NetworkType & aType, NetworkAbstractParams * apNetworkParams) {

	NetworkAbstract * pNet = NULL;

	switch (aType) {
	case NetworkType::BA:
		pNet = new NetworkBA(apNetworkParams->m_nConnectedNodes);
		break;
	case NetworkType::ER:
		pNet = new NetworkER(apNetworkParams->m_dbConnectionProb);
		break;
	case NetworkType::FC:
		pNet = new NetworkFC();
		break;
	case NetworkType::SW:
		pNet = new NetworkSW(apNetworkParams->m_nNeighbors,apNetworkParams->m_dbRewiringProb);
		break;
	case NetworkType::SWAge:
		pNet = new NetworkSWAge(apNetworkParams->m_nNeighbors, apNetworkParams->m_dbRewiringProb);
		break;
	case NetworkType::CM:
		pNet = new NetworkCM(apNetworkParams->m_nNeighbors);
		break;

	default:
		return NULL;
	}

	return pNet;
}

//-------------------------------------------------------------------------------------------------------

void NetworkAbstract::setOpiformModel(const OpinionFormationModel::OpinionFormationModelType & aType) {
	m_OpiformModel = &OpinionFormationModel::getFunction(aType);
}

//-------------------------------------------------------------------------------------------------------

bool NetworkAbstract::isConnected(const std::vector<AgentBase*> * apvecAgents) {

	/*
		A connected network can access any point from any initial point
	*/

	int nSize = apvecAgents->size();

	//Take a random initial node
	std::uniform_int_distribution<> dis(0,nSize-1);
	AgentBase * pA = (*apvecAgents)[dis(gen)];

	queue<AgentBase*> qAgents;
	qAgents.push(pA);
	int nCount = 1;

	vector<int> vecIds(nSize, -1);
	vecIds[pA->getID()] = 1;

	while (!qAgents.empty()) {
		pA = qAgents.front();
		qAgents.pop();

		//Iterate through children
		int nChildren = pA->getNeighborhoodSize();
		for (int nI = 0; nI < nChildren; ++nI) {
			AgentBase * pAC = (AgentBase *)pA->getNeighbor(nI);
			int nIAC = pAC->getID();
			if (vecIds[nIAC] != 1) {
				vecIds[nIAC] = 1;
				qAgents.push(pAC);
				++nCount;
			}
		}
	}

	return (nCount == nSize);
}

//-----------------------------------------------------------------------------------------------------

bool NetworkAbstract::getEdgeList(const std::vector<AgentBase*> & avecAgents, std::vector<Edge> & avecEdgeList) {

	for (AgentBase * pel : avecAgents) {
		int nId = pel->getID();
		int nSize = pel->getNeighborhoodSize();

		for (int nI = 0; nI < nSize; ++nI) {
			Edge e;
			e.m_nSourceNode = nId;
			e.m_nTargetNode = pel->getNeighbor(nI)->getID();
			avecEdgeList.push_back(e);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------------------------------

bool NetworkAbstract::getAdjacencyList(const vector<AgentBase*> & avecAgents, map<int, vector<int> > & amapAdjacencyList)	{
	for (AgentBase * pel : avecAgents) {
		int nId = pel->getID();
		int nSize = pel->getNeighborhoodSize();

		vector<int> vecT(nSize);

		for (int nI = 0; nI < nSize; ++nI) {
			vecT[nI] = pel->getNeighbor(nI)->getID();
		}
		amapAdjacencyList[pel->getID()] = vecT;
	}

	return true;
}

//------------------------------------------------------------------------------------------------------

