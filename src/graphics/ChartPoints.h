#ifndef SRC_GRAPHICS_CHARTPOINTS_H_
#define SRC_GRAPHICS_CHARTPOINTS_H_

#include <xcb/xcb.h>
#include "utils/LinkedList.h"

namespace graphics {

/**
 * Points chart container.
 * Transform each input point {double, double} to output ploter coordinates
 */
class ChartPoints {
public:
	ChartPoints(const xcb_rectangle_t& _boundRect) :
			boundRect(_boundRect), xRatio(1), yRatio(1), inrange(
					{ 0, 0, 0, 0 }) {
		outpoints = new utils::LinkedList<Point>();
	}

	~ChartPoints() {
		delete outpoints;
	}

	struct Point {
		double x;
		double y;
	};

	struct Range {
		double xMax;
		double yMax;
		double xMin;
		double yMin;
	};

	void addPoint(double x, double y);

private:
	utils::LinkedList<Point>* outpoints;
	xcb_rectangle_t boundRect;

	Range inrange;

	// compression ratio
	double xRatio; // boundRect.width/(inrange.xMax-inrange.xMin)
	double yRatio;

	bool calculateRange(const double& x, const double& y);
	void calculateRatio();

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CHARTPOINTS_H_ */
