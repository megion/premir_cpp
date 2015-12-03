#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <xcb/xcb.h>

#include "graphics/Chart.h"


double get_time(void) {
	struct timeval timev;

	gettimeofday(&timev, NULL);

	return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
}

void test_chart() {
	graphics::Chart chart;
	chart.runChart();
	chart.addPoint(100, 100);
	chart.addPoint(101, 100);
	chart.addPoint(102, 100);
}
