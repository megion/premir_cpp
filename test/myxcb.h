#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <random>
#include <chrono>         // std::chrono::seconds

#include <xcb/xcb.h>

#include "graphics/ChartThread.h"
#include "ml/DigitalFilter.h"

double get_time(void) {
	struct timeval timev;

	gettimeofday(&timev, NULL);

	return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
}

#define PI 3.14159265

void test_chart() {
	graphics::ChartThread chart(860, 660);
	graphics::ChartThread chart2(860, 660);

//	double param, result;
//	param = 30.0;

	ml::DigitalFilter df(100);

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0,0.3);

	graphics::ChartData* chartData = chart.getChart().getData();
	graphics::ChartData* chartData2 = chart2.getChart().getData();



	for (double i = 0; i < 1000.0; i = i + 0.1) {
		double x = sin(i * PI / 180);//pow(i, 4) + 100*pow(i, 3) + pow(i,2) + i;
		double n = distribution(generator);
		//		double result = pow(i, 4) + 100*pow(i, 3) + pow(i,2) + i;
		double result = cos(i * PI / 280)*n + x;


		if (!isnan(result)) {
//			std::cout << "result: " << result << std::endl;


			df.addInput(result);
			double s = df.calculateSum();

//			chartData->addPoint(i, s);

//			chart.getChart().drawAxis();
//			chart.getChart().drawPoints();
//			chart.getChart().flush();
//
			chart.getChart().redrawNewPoints(i, s);
			chart.getChart().flush();
			chart2.getChart().redrawNewPoints(i, result);
						chart2.getChart().flush();
//			chart.getChart().redrawNewPoints(i, -result);
//			chart.getChart().flush();

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
//			chart.getChart().drawBackground();

		}
	}

	chart.getChart().draw();
	chart.getChart().flush();
	chart2.getChart().draw();
		chart2.getChart().flush();

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
