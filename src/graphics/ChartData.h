#ifndef SRC_GRAPHICS_CHARTDATA_H_
#define SRC_GRAPHICS_CHARTDATA_H_

#include <xcb/xcb.h>
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <exception>
#include <stdexcept>
#include <errno.h>
#include <iostream>
#include "utils/LinkedList.h"

namespace graphics {

/**
 * Points chart container.
 * Transform each input point {double, double} to output ploter coordinates
 */
class ChartData {
public:
	ChartData(const xcb_rectangle_t& _boundRect) :
			boundRect(_boundRect), xRatio(1), yRatio(1), inrange(
					{ 0, 0, 0, 0 }), outpoints(nullptr) {
		inpoints = new utils::LinkedList<Point>();
		std::cout << "Create chart data " << std::endl;
	}

	~ChartData() {
		delete inpoints;
		if (outpoints) {
			free(outpoints);
			outpoints = nullptr;
		}
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

	xcb_point_t* getOutpoints() {
		return outpoints;
	}

	size_t size() {
		return inpoints->size();
	}

	void addPoint(double x, double y);
	void printPoints();

private:
	utils::LinkedList<Point>* inpoints;
	xcb_point_t* outpoints;
	xcb_rectangle_t boundRect;

	Range inrange;

	// compression ratio
	double xRatio; // boundRect.width/(inrange.xMax-inrange.xMin)
	double yRatio;

	/**
	 * Calculate value by formula (line) out = a*value + b
	 */
	int line(double a, double b, double value) {
		return std::round(a * value + b);
	}

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CHARTDATA_H_ */
