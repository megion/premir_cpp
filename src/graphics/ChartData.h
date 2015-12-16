#ifndef SRC_GRAPHICS_CHART_DATA_H_
#define SRC_GRAPHICS_CHART_DATA_H_

#include <xcb/xcb.h>
#include <math.h>

#include <cstdio>
#include <exception>
#include <stdexcept>
#include <cerrno>
#include <iostream>
#include <limits>

#include "utils/CArrayList.h"

namespace graphics {

/**
 * Data chart container.
 * Transform each input point {double, double} to output ploter coordinates
 */
class ChartData {
public:
	ChartData(const xcb_rectangle_t& _boundRect) :
			boundRect(_boundRect), inrange({ 0, 0, 0, 0 }), //
			outpoints(nullptr) {
		inpoints = new utils::CArrayList<Point>(0, 40);
		outpoints = new utils::CArrayList<xcb_point_t>(0, 40);
		createAxesPoints();
	}

	~ChartData() {
		delete inpoints;
		delete xAxes;
		delete yAxes;
		delete outpoints;
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

	utils::CArrayList<xcb_point_t>* getOutpoints() {
		return outpoints;
	}

	utils::CArrayList<Axis>* getXAxes() {
		return xAxes;
	}

	utils::CArrayList<Axis>* getYAxes() {
		return yAxes;
	}

	size_t size() {
		return inpoints->size();
	}

	void addPoint(double x, double y);
	void updateAxesLabel(double ax, double bx, double ay, double by);
	void printPoints();
	void removeData();

private:
	utils::CArrayList<Point>* inpoints;
	utils::CArrayList<Axis>* yAxes;
	utils::CArrayList<Axis>* xAxes;
	utils::CArrayList<xcb_point_t>* outpoints;
	xcb_rectangle_t boundRect;

	Range inrange;

	/**
	 * Calculate value by formula (line) out = a*value + b
	 */
	int transformToOut(double a, double b, double inValue) {
		return round(a * inValue + b);
	}
	double transformToIn(double a, double b, int16_t outValue) {
		return ((double)(outValue) - b)/a;
	}

	void createAxesPoints();

};

} /* namespace graphics */

#endif
