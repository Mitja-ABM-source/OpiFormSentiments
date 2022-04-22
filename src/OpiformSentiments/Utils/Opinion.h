#ifndef OPINION_H
#define OPINION_H

#include <map>

namespace opiform {

	class Opinion {
	public:
		/*
		enum OpinionPosition	{
			StronglyDisapprove = 0,
			Disapprove,
			Undecided,
			Approve,
			StronglyApprove,
			S6,
			S7,
			S8,
			S9,
			S10
		};
		static const int getNumOpinionPositions() { return 10; };
*/
		typedef double OpinionPosition;

		enum OpinionTopic {
			OpinionTopic_0,
			/*
			OpinionTopic_1,
			OpinionTopic_2,
			OpinionTopic_3,
			OpinionTopic_4,
			OpinionTopic_5,
			OpinionTopic_6,
			OpinionTopic_7,
			OpinionTopic_8,
			OpinionTopic_9*/
		};
		static const int getNumTopics() { return 1; };
		static Opinion::OpinionTopic getTopic();


	public:
		Opinion();
		~Opinion();

		//------------------------------------------------------------------------------------------------------------------

		typedef enum {
			Euclidian = 0,
			EuclidianFermi,
			Jensen
		} DistanceMeasure;

		typedef std::map<Opinion::OpinionTopic, Opinion::OpinionPosition> mapOpinionTopicPosition;

		typedef double(*Func)(const mapOpinionTopicPosition &, const mapOpinionTopicPosition &);

		static std::map<DistanceMeasure, Func> s_mapDistanceMeasures;

		static void registerType(const DistanceMeasure & anType, const Func & aFunc) {
			s_mapDistanceMeasures[anType] = aFunc;
		};


		static inline double calcDistance(
			const mapOpinionTopicPosition & aOTP1, const mapOpinionTopicPosition & aOTP2,
			const DistanceMeasure & aMethod = Opinion::Euclidian) {

			Func * pFunc = &s_mapDistanceMeasures[aMethod];

			return (*pFunc)(aOTP1, aOTP2);
		}

		//----------------------------------------------------------------------------------------------------------

		static inline double euclidianDist(const mapOpinionTopicPosition & aOTP1, const mapOpinionTopicPosition & aOTP2) {

			double dbDist = 0.0;
			int nSize = aOTP1.size();

			for (auto & el1 = aOTP1.cbegin(), el2 = aOTP2.cbegin(); el1 != aOTP1.cend(); ++el1,++el2) {
				dbDist += ((el1->second - el2->second) * (el1->second - el2->second));
			}

			return dbDist;
		}

		//----------------------------------------------------------------------------------------------------------

		static inline double euclidianFermiDist(const mapOpinionTopicPosition & aOTP1, const mapOpinionTopicPosition & aOTP2) {

			double dbDist = 0.0;
			int nSize = aOTP1.size();

			for (auto & el1 = aOTP1.cbegin(), el2 = aOTP2.cbegin(); el1 != aOTP1.cend(); ++el1, ++el2) {
				dbDist += ((el1->second - el2->second) * (el1->second - el2->second));
			}

			return dbDist;
		}

		//------------------------------------------------------------------------------------------------------------------

		static inline double jensenDist(const mapOpinionTopicPosition & aOTP1, const mapOpinionTopicPosition & aOTP2) {

			double dbDist = 0.0;
			double dbMin = std::numeric_limits<double>::min();

			for (auto & el1 = aOTP1.cbegin(), el2 = aOTP2.cbegin(); el1 != aOTP1.cend(); ++el1, ++el2) {
				double dbT0 = (
					el1->second * log(((double)el1->second + dbMin)) +
					el2->second * log(((double)el2->second + dbMin))
				) / 2.0;

				double dbT1 = (el1->second + el2->second) / 2.0;
				dbT1 *= log(dbT1 + dbMin);

				dbDist += (dbT0 - dbT1);
			}

			return dbDist;
		}

	};
}
#endif