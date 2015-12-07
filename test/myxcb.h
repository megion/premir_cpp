#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <chrono>         // std::chrono::seconds

#include <xcb/xcb.h>

#include "graphics/ChartThread.h"

double get_time(void) {
	struct timeval timev;

	gettimeofday(&timev, NULL);

	return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
}

#define PI 3.14159265

void test_chart() {
	graphics::ChartThread chart(860, 660);

//	double param, result;
//	param = 30.0;

	graphics::ChartData* chartData = chart.getChart().getData();

	for (double i = -100; i <= 100.0; i = i + 0.5) {
		double result = pow(i, 4) + 100*pow(i, 3) + pow(i,2) + i;
		if (!isnan(result)) {
//			std::cout << "result: " << result << std::endl;
//			chartData->addPoint(i, result);
//			chartData->addPoint(i, -result);

//			chart.getChart().drawAxis();
//			chart.getChart().drawPoints();
//			chart.getChart().flush();
			chart.getChart().redrawNewPoints(i, result);
//			chart.getChart().flush();
//			chart.getChart().redrawNewPoints(i, -result);
			chart.getChart().flush();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
//			chart.getChart().drawBackground();

		}
	}

	chart.getChart().draw();
	chart.getChart().flush();

//	graphics::ChartThread chart2(560, 460);
//
//	graphics::ChartData* chartData2 = chart2.getChart().getData();
//	chartData2->addPoint(10, 10);
//	chartData2->addPoint(20, 20);
//	chartData2->addPoint(30, 10);
//
//	chart2.getChart().draw();
//	chart2.getChart().flush();

//	chart.addPoint(1, 1);
//	chart.addPoint(2, 2);
//	chart.addPoint(100, 100);
//	chart.addPoint(101, 100);
//	chart.addPoint(102, 100);
//	chart.addPoint(1000, 1000);
//	chart.printPoints();
//	chart.runChart();

}
