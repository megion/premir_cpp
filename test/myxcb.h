#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#include <xcb/xcb.h>

#include "graphics/Chart.h"

double get_time(void) {
	struct timeval timev;

	gettimeofday(&timev, NULL);

	return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
}

#define PI 3.14159265

void test_chart() {
	graphics::Chart chart(460, 360);

//	double param, result;
//	param = 30.0;

	for (double i = -6.0; i <= 6.0; i = i + 0.01) {
		double result = pow((25 - i * i), 0.5); //sin(i * PI / 180);
		if (!isnan(result)) {
//			std::cout << "result: " << result << std::endl;
			chart.addPoint(i, result);
			chart.addPoint(i, -result);
		}

	}

//	chart.addPoint(1, 1);
//	chart.addPoint(2, 2);
//	chart.addPoint(100, 100);
//	chart.addPoint(101, 100);
//	chart.addPoint(102, 100);
//	chart.addPoint(1000, 1000);
//	chart.printPoints();
	chart.runChart();

}
