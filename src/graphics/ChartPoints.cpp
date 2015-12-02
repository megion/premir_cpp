#include "ChartPoints.h"

namespace graphics {

void ChartPoints::addPoint(double x, double y) {
	// 1. add point
	Point p = { x, y };
	inpoints->push(p);

	// 2. calculate range
	bool xChanged = false;
	if (x > inrange.xMax) {
		inrange.xMax = x;
		xChanged = true;
	} else if (x <= inrange.xMin) {
		inrange.xMin = x;
		xChanged = true;
	}
	bool yChanged = false;
	if (y > inrange.yMax) {
		inrange.yMax = y;
		yChanged = true;
	} else if (y <= inrange.yMin) {
		inrange.yMin = y;
		yChanged = true;
	}

	// 3. calculate line
	// x line: xout = ax*xin + bx
	// ax = w0/(xmax-xmin); bx = x0 - ax*xmin
	double xdiff = inrange.xMax - inrange.xMin;
	double ax = ((double) boundRect.width) / xdiff;
	double bx = ((double) boundRect.x) - ax * inrange.xMin;

	// y line: yout = ay*yin + by
	// ay = h0/(ymax-ymin); by = y0 - h0*ymin/(ymax-ymin)
	double ydiff = inrange.yMax - inrange.yMin;
	double ay = ((double) boundRect.height) / ydiff;
	double by = ((double) boundRect.y) - ay * inrange.yMin;

	// 4. copy out point array to new array
	// use memcpy for best performance
//	int size = inpoints->size();
//	if (outpoints) {
//		xcb_point_t* newOutpoints = malloc(size * sizeof(xcb_point_t));
//		memcpy(outpoints, newOutpoints, size * sizeof(xcb_point_t));
//		free(outpoints); // remove old
//		outpoints = newOutpoints;
//	} else {
//		outpoints = malloc(sizeof(xcb_point_t));
//	}

}

}

