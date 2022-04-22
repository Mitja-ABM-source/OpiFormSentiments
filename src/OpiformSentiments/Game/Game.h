#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <queue>

namespace opiform {

	class Game	{
	private:
		std::vector <class AgentBase * > m_vecAgents;
		std::string m_strFileName;

	public:
		Game(const std::string & astrFileName);
		~Game();

		bool init(int anSize = 200, double adbMu = 0.8, double adbKappaMean = 0.5, double adbGammaMean = 0.5, double adbThetaMean = 0.5);
		bool initNetwork(const NetworkType & aNetworkType, struct NetworkAbstractParams * apNetworkParams = 0);
		void runGame(const time_t & aTime);

		static void registerStatics();

		int getAgents() const;

	private:
		void initAgents(int anSize, const double & adbMu, const double & adbKappaMean, const double & adbGammaMean, const double & adbThetaMean);
		std::queue<int> listDeceased();

		void updateRanking();
	};
}

#endif
