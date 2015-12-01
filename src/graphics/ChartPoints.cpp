#include "ChartPoints.h"

namespace graphics {

void ChartPoints::addPoint(double x, double y) {
	bool changed = calculateRange(x, y);
	if (changed) {
		calculateRatio();
	}
}

bool ChartPoints::calculateRange(const double& x, const double& y) {
	bool isChanged = false;
	if (x > inrange.xMax) {
		inrange.xMax = x;
		isChanged = true;
	} else if (x <= inrange.xMin) {
		inrange.xMin = x;
		isChanged = true;
	}

	if (y > inrange.yMax) {
		inrange.yMax = y;
		isChanged = true;
	} else if (y <= inrange.yMin) {
		inrange.yMin = y;
		isChanged = true;
	}

	return isChanged;
}

void ChartPoints::calculateRatio() {
}

}

