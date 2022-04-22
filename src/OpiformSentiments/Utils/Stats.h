#ifndef OPIFORM_STATS_H
#define OPIFORM_STATS_H

#include <ostream>
#include <deque>

#include "Opinion.h"

namespace opiform {

	struct Statistics {
		double m_dbMean;
		double m_dbVar;
		int m_nMeetingsNumber;
		int m_nOpinionChangeNumber;
		double m_dbOverallOpinionChange;
	};

	class Stats {
	public:
		static bool analyze(std::vector<class AgentBase*> & avecAgents, std::vector<Statistics> & avecStatsOut);

		static double covar(const Opinion::mapOpinionTopicPosition & aOTP1, const Opinion::mapOpinionTopicPosition & aOTP2);
		static double varKnuth(const std::deque<double> &);
	};
}

#endif