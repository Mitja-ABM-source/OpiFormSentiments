#include "constants.h"

namespace opiform {
	//https://www.ssa.gov/oact/STATS/table4c6.html#fn1
	typedef std::pair<int, double> lifeTablePair;
	const lifeTablePair mapValues[] = {
		lifeTablePair(20 * 12, 8.33715521976197E-05),
		lifeTablePair(30 * 12, 8.33715521976197E-05),
		lifeTablePair(40 * 12, 0.0001668196),
		lifeTablePair(50 * 12, 0.0004176246),
		lifeTablePair(60 * 12, 0.0009213209),
		lifeTablePair(70 * 12, 0.0020223433),
		lifeTablePair(80 * 12, 0.0050548593),
		lifeTablePair(90 * 12, 0.0151114615),
		lifeTablePair(100 * 12, 0.0357580026),
		lifeTablePair(110 * 12, 0.0690054156)
	};

	const int sz = sizeof(mapValues) / sizeof(mapValues[0]);
	std::map<int, double > opiform::mapLifeTable(mapValues,mapValues + sz);
};


//	static void initMap() {
//mapLifeTable[20 * 12] = 8.33715521976197E-05;
//mapLifeTable[30 * 12] = 8.33715521976197E-05;
//mapLifeTable[40 * 12] = 0.0001668196;
//mapLifeTable[50 * 12] = 0.0004176246;
//mapLifeTable[60 * 12] = 0.0009213209;
//mapLifeTable[70 * 12] = 0.0020223433;
//mapLifeTable[80 * 12] = 0.0050548593;
//mapLifeTable[90 * 12] = 0.0151114615;
//mapLifeTable[100 * 12] = 0.0357580026;
//mapLifeTable[110 * 12] = 0.0690054156;
//	};


//yearly data
//{ 20, 0.001 },
//{ 30, 0.001 },
//{ 40, 0.002 },
//{ 50, 0.005 },
//{ 60, 0.011 },
//{ 70, 0.024 },
//{ 80, 0.059 },
//{ 90, 0.167 },
//{ 100, 0.354 },
//{ 110, 0.576 }

//Monthly
//{ 20 * 12, 8.33715521976197E-05 },
//{ 30 * 12, 8.33715521976197E-05 },
//{ 40 * 12, 0.0001668196 },
//{ 50 * 12, 0.0004176246 },
//{ 60 * 12, 0.0009213209 },
//{ 70 * 12, 0.0020223433 },
//{ 80 * 12, 0.0050548593 },
//{ 90 * 12, 0.0151114615 },
//{ 100 * 12, 0.0357580026 },
//{ 110 * 12, 0.0690054156 }

//DAILY - recalculated as Binomial random variable
//{ 20*365, 2.74109304754244E-06 },
//{ 30*365, 2.74109304754244E-06 },
//{ 40*365, 5.48492378149579E-06 },
//{ 50*365, 1.37328970002404E-05 },
//{ 60*365, 3.03035062078072E-05 },
//{ 70*365, 6.65531073507086E-05 },
//{ 80*365, 0.0001665947 },
//{ 90*365, 0.0005004819 },
//{ 100 * 365, 0.0011964228 },
//{ 110 * 365, 0.0023479839 }

//	};
