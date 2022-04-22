#ifndef POLITICAL_VIEW_H
#define POLITICAL_VIEW_H

#include <vector>
#include "Opinion.h"

namespace opiform {
	struct PoliticalView {
		PoliticalView(int anID, const Opinion::mapOpinionTopicPosition & amapOpinionTopicPosition);

	private:
		int m_nID;
		Opinion::mapOpinionTopicPosition m_mapOpinionTopicPosition;

	public:
		int getID() const { return m_nID; }
		const Opinion::mapOpinionTopicPosition * getOpinionTopicPositions() const { return &m_mapOpinionTopicPosition; }

		void updatePosition() {};
	};
}
#endif