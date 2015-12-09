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
#include "utils/LinkedList.h"

double get_time(void) {
	struct timeval timev;

	gettimeofday(&timev, NULL);

	return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
}

#define PI 3.14159265

void test_chart() {
	graphics::ChartThread chartSignalTemplate(680, 460);
	graphics::ChartThread chartSignalIn(680, 460);
	graphics::ChartThread chartSignalOut(680, 460);
	graphics::ChartThread chartFilterWeights(680, 460);

	chartSignalTemplate.getChart().setWindowTitle("Signal template",
			"Signal template");
	chartSignalIn.getChart().setWindowTitle("Signal+noise", "Signal+noise");
	chartSignalOut.getChart().setWindowTitle("Signal out", "Signal out");
	chartFilterWeights.getChart().setWindowTitle("FilterWeights",
			"FilterWeights");

//	double param, result;
//	param = 30.0;

	ml::DigitalFilter df(800, 0.00001);

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 0.3);

	for (double i = 0; i < 10000.0; i = i + 1) {
		double noise = 5*cos((i+180) * PI / 180); //pow(i, 4) + 100*pow(i, 3) + pow(i,2) + i;
//		double noise = distribution(generator);
//				double noise = pow(i, 4) + pow(i, 3) + pow(i,2) + i;
//		double noise = (double)(((int)i)%33);

		double signal = 5 * cos(i * PI / 180);

		double signalAndNoise = signal + noise;

		if (!isnan(signalAndNoise)) {
//			std::cout << "result: " << result << std::endl;

			df.addInput(noise);
			double noiseEval = df.calculateFilterOutSum(); // оценка шума
			double signalEval = df.updateFilterWeightsByLeastSquaresAlgorithm(
					signalAndNoise, noiseEval);

//			chartData->addPoint(i, s);

//			chart.getChart().drawAxis();
//			chart.getChart().drawPoints();
//			chart.getChart().flush();
//
			chartSignalTemplate.getChart().redrawNewPoints(i, signal);
			chartSignalTemplate.getChart().flush();

			chartSignalIn.getChart().redrawNewPoints(i, signalAndNoise);
			chartSignalIn.getChart().flush();

			chartSignalOut.getChart().redrawNewPoints(i, signalEval);
			chartSignalOut.getChart().flush();

			chartFilterWeights.getChart().getData()->removeData();
			utils::LinkedList<double>* weightsArray =
					df.getWeightsArray();
			utils::LinkedList<double>::Iterator iter =
					weightsArray->iterator();
			double k = 0.0;
			while (iter.hasNext()) {
				utils::LinkedList<double>::Entry* e =
						iter.next();
				double& w = e->getValue();
				chartFilterWeights.getChart().getData()->addPoint(k, w);
				++k;
			}
			chartFilterWeights.getChart().drawBackground();
			chartFilterWeights.getChart().drawAxes();
			chartFilterWeights.getChart().drawAxesLabels();
			chartFilterWeights.getChart().drawPoints();
			chartFilterWeights.getChart().flush();

//			chart.getChart().redrawNewPoints(i, -result);
//			chart.getChart().flush();

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
//			chart.getChart().drawBackground();

		}
	}

//	chartSignalIn.getChart().draw();
//	chartSignalIn.getChart().flush();
//
//	chart2.getChart().draw();
//	chart2.getChart().flush();

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
