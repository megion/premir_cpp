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
	size_t amount = inpoints->size() * sizeof(struct xcb_point_t);
	if (outpoints) {
		xcb_point_t* newOutpoints = (struct xcb_point_t*) realloc(outpoints,
				amount);
		if (newOutpoints == NULL) {
			throw std::runtime_error(strerror(errno));
		}
		outpoints = newOutpoints;
	} else {
		outpoints = (struct xcb_point_t*) malloc(amount);
		if (outpoints == NULL) {
			throw std::runtime_error(strerror(errno));
		}
	}

	// 5. update outpoints array
	utils::LinkedList<Point>::Iterator iter = inpoints->iterator();
	size_t  i = 0;
	while (iter.hasNext()) {
		utils::LinkedList<Point>::Entry* e = iter.next();
		Point& inp = e->getValue();
		std::cout << "outp.x: " << inp.x << std::endl;
		outpoints[i].x = (int)inp.x;
		outpoints[i].y = (int)inp.y;
		++i;
	}

}

}

