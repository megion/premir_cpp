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
#include <limits>

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
		xAxes = new utils::LinkedList<Axis>();
		yAxes = new utils::LinkedList<Axis>();

		createAxesPoints();
	}

	~ChartData() {
		delete inpoints;
		delete xAxes;
		delete yAxes;
		if (outpoints) {
			free(outpoints);
			outpoints = nullptr;
		}
	}

	struct Point {
		double x;
		double y;
	};

	struct Axis {
		xcb_point_t line[2];
		xcb_point_t labelPoint;
		double labelValue;
		bool hideLabel;
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

	utils::LinkedList<Axis>* getXAxes() {
		return xAxes;
	}

	utils::LinkedList<Axis>* getYAxes() {
		return yAxes;
	}

	size_t size() {
		return inpoints->size();
	}

	void addPoint(double x, double y);
	void printPoints();

private:
	utils::LinkedList<Point>* inpoints;
	// TODO axis replace to ArrayList with const size
	utils::LinkedList<Axis>* yAxes;
	utils::LinkedList<Axis>* xAxes;
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

	void createAxesPoints();

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CHARTDATA_H_ */
