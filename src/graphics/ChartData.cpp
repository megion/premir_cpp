#include <ChartData.h>

namespace graphics {

void ChartData::addPoint(double x, double y) {
	// 1. add point
	Point p = { x, y };
	inpoints->push(p);

	// 2. copy out point array to new array
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

	// 3. calculate range
	// start coordinate point apply left-down corner = (x0, y0 + h0)

	if (inpoints->size() == 1) {
		// first point so simple do initialize
		inrange.xMax = x;
		inrange.xMin = x;
		inrange.yMax = y;
		inrange.yMin = y;

		// start coordinates (x0, y0 + h0)
		outpoints[0].x = boundRect.x;
		outpoints[0].y = boundRect.y + boundRect.height;
		return;
	}

	// if isDirty = true then should recalculate all points
	bool xIsDirty = false;
	bool yIsDirty = false;

	if (x > inrange.xMax) {
		inrange.xMax = x;
		xIsDirty = true;
	} else if (x < inrange.xMin) {
		inrange.xMin = x;
		xIsDirty = true;
	}

	if (y > inrange.yMax) {
		inrange.yMax = y;
		yIsDirty = true;
	} else if (y < inrange.yMin) {
		inrange.yMin = y;
		yIsDirty = true;
	}

	// 4. calculate line
	size_t lastIndex = inpoints->size() - 1;
	double ax = 0, bx = 0;
	double ay = 0, by = 0;
	bool xIsConst = false;
	if (inrange.xMax == inrange.xMin) {
		// x has not ever been change, set to x0
		outpoints[lastIndex].x = boundRect.x;
		xIsConst = true;
	} else {
		// x line: xout = ax*xin + bx
		// ax = w0/(xmax-xmin); bx = x0 - ax*xmin
		double xdiff = inrange.xMax - inrange.xMin;
		ax = ((double) boundRect.width) / xdiff;
		bx = ((double) boundRect.x) - ax * inrange.xMin;
	}

	bool yIsConst = false;
	if (inrange.yMax == inrange.yMin) {
		// y has not ever been change, set to y0 + h0
		outpoints[lastIndex].y = boundRect.y + boundRect.height;
		yIsConst = true;
	} else {
		// y line: yout = ay*yin + by
		// ay = -h0/(ymax-ymin); by = (y0+h0) - ay*ymin
		double ydiff = inrange.yMax - inrange.yMin;
		ay = -((double) boundRect.height) / ydiff;
		by = ((double)(boundRect.y + boundRect.height))
				- ay * inrange.yMin;
	}

	if (xIsConst && yIsConst) {
		return;
	}

	// 5. update outpoints array
	if (!xIsDirty && !yIsDirty) {
		// range input values no changes so calculate only current point
		if (!xIsConst) {
			outpoints[lastIndex].x = line(ax, bx, x);
		}
		if (!yIsConst) {
			outpoints[lastIndex].y = line(ay, by, y);
		}
	} else {
		utils::LinkedList<Point>::Iterator iter = inpoints->iterator();
		size_t i = 0;
		while (iter.hasNext()) {
			utils::LinkedList<Point>::Entry* e = iter.next();
			Point& inp = e->getValue();
			// recalculate all if IsDirty else only last is recalculate
			if (!xIsConst && (xIsDirty || (i==lastIndex))) {
				outpoints[i].x = line(ax, bx, inp.x);
			}
			if (!yIsConst && (yIsDirty || (i==lastIndex))) {
				outpoints[i].y = line(ay, by, inp.y);
			}
			++i;
		}
	}

}

void ChartData::printPoints() {
	for (size_t i = 0; i < inpoints->size(); ++i) {
		std::cout << "outp.x: " << outpoints[i].x << " outp.y: "
				<< outpoints[i].y << std::endl;
	}
}

}

