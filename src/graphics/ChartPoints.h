#ifndef SRC_GRAPHICS_CHARTPOINTS_H_
#define SRC_GRAPHICS_CHARTPOINTS_H_

#include "utils/LinkedList.h"

namespace graphics {

class ChartPoints {
public:
	ChartPoints() {
		inpoints = utils::LinkedList<InPoint>();
	}

	~ChartPoints() {

	}

	struct InPoint {
		double x;
		double y;
	};
	struct OutPoint {
		int x;
		int y;
	};

private:
	utils::LinkedList<InPoint> inpoints;
};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CHARTPOINTS_H_ */
