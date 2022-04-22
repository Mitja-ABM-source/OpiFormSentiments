#ifndef NETWORK_ABSTRACT_H
#define NETWORK_ABSTRACT_H

#include <vector>
#include <memory>

#include "../Utils/OpinionFormationModels.h"

namespace opiform {

	enum NetworkType {
		SW = 0,		//Small-world (Watts-Strogatz)
		FC,			//Fully-connected
		ER,			//Erdos–Renyi
		BA,			//Barabasi-Albert
		CM,			//Communities
		SWAge
	};

	struct NetworkAbstractParams {
		int m_nNeighbors /*= 10*/;
		int m_nConnectedNodes;
		double m_dbConnectionProb;
		double m_dbRewiringProb;
		OpinionFormationModel::OpinionFormationModelType m_OpinionFormationModelType;
	};

	typedef struct {
		int m_nSourceNode;
		int m_nTargetNode;
	} Edge;

	class NetworkAbstract {
	public:
		NetworkAbstract() {}
		virtual ~NetworkAbstract() = 0;

		virtual NetworkType getNetworkType() const = 0;

		virtual bool generateNetwork(std::vector<class AgentBase *> * apvecAgents) = 0;

		static NetworkAbstract * findAndCreateNetwork(const NetworkType & aNetworkType, NetworkAbstractParams * apNetworkParams = 0);

		virtual bool step(std::vector<class AgentBase *> * apvecAgents) = 0;
		void setOpiformModel(const OpinionFormationModel::OpinionFormationModelType & aType);

		static bool isConnected(const std::vector<class AgentBase*> * apvecAgents);

		bool getEdgeList(const std::vector<class AgentBase*> & avecAgents, std::vector<Edge> & avecEdgeList);
		bool getAdjacencyList(const std::vector<class AgentBase*> & avecAgents, std::map<int,std::vector<int> > & amapAdjacencyList);

	protected:
		OpinionFormationModel::OpinionFormationModelFunc * m_OpiformModel;
	};
}

#endif