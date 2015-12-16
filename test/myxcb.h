//#include <stdlib.h>
//#include <stdio.h>
//#include <sys/time.h>

#include <cmath>
#include <random>
#include <chrono>         // std::chrono::seconds

#include <xcb/xcb.h>

#include "graphics/ChartThread.h"
#include "ml/DigitalFilter.h"
#include "utils/LinkedList.h"
#include "utils/CArrayList.h"


void test_chart() {
	graphics::ChartThread chartSignalTemplate(710, 460);
	graphics::ChartThread chartSignalIn(710, 460);
//	graphics::ChartThread chartNoise(680, 460);
	graphics::ChartThread chartNoiseEval(710, 460);
	graphics::ChartThread chartSignalOut(710, 460);
	graphics::ChartThread chartFilterWeights(710, 460);

	chartSignalTemplate.getChart().setWindowTitle("Signal template");
	chartSignalIn.getChart().setWindowTitle("Signal+noise");
//	chartNoise.getChart().setWindowTitle("Noise");
	chartNoiseEval.getChart().setWindowTitle("Noise eval");
	chartSignalOut.getChart().setWindowTitle("Signal out");
	chartFilterWeights.getChart().setWindowTitle("Weights");

	double TN = 500.0; // кол-во точек на период
	size_t N = 1200; // размеры
	double sampleStep = 2*M_PI/TN;
	double speed = 1.0/(N*36.0*10.0);

	ml::DigitalFilter df(N, speed, 0.0);
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 0.1);

	for (double i = 0.0; i <= 2*M_PI*10.0; i = i + sampleStep) {
//		double noise = distribution(generator)
//				+ 2.0 * cos(8.0 * i * PI / 180.0);
		double noise = 2.0 * cos(4.0 * i);
		//pow(i, 4) + 100*pow(i, 3) + pow(i,2) + i;
//		double noise = distribution(generator);
//				double noise = pow(i, 4) + pow(i, 3) + pow(i,2) + i;
//		double noise = (double)(((int)i)%33);

		double signal = 4.0 * cos(i);

		double signalAndNoise = signal + noise;

		if (!std::isnan(signalAndNoise)) {
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
			chartNoiseEval.getChart().redrawNewPoints(i, noiseEval);
			chartSignalIn.getChart().redrawNewPoints(i, signalAndNoise);
//			chartNoise.getChart().redrawNewPoints(i, noise);
			chartSignalOut.getChart().redrawNewPoints(i, signalEval);

//			chartSignalTemplate.getChart().flush();
//			chartNoiseEval.getChart().flush();
//			chartSignalIn.getChart().flush();
////			chartNoise.getChart().flush();
//			chartSignalOut.getChart().flush();

//			chartSignalTemplate.getChart().getData()->addPoint(i, signal);
//			chartNoiseEval.getChart().getData()->addPoint(i, noiseEval);
//			chartSignalIn.getChart().getData()->addPoint(i, signalAndNoise);
//			chartNoise.getChart().getData()->addPoint(i, noise);
//			chartSignalOut.getChart().getData()->addPoint(i, signalEval);

			///////// draw filter weight
			chartFilterWeights.getChart().getData()->removeData();
			double k = 0.0;
			for (double& w : (*df.getWeightsArray())) {
				chartFilterWeights.getChart().getData()->addPoint(k, w);
				++k;

			}
			chartFilterWeights.getChart().drawBackground();
			chartFilterWeights.getChart().drawAxes();
			chartFilterWeights.getChart().drawAxesLabels();
			chartFilterWeights.getChart().drawPoints();
			chartFilterWeights.getChart().flush();
			/////////

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
//			chart.getChart().drawBackground();

		}
	}

	chartSignalTemplate.getChart().draw();
	chartNoiseEval.getChart().draw();
	chartSignalIn.getChart().draw();
//	chartNoise.getChart().draw();
	chartSignalOut.getChart().draw();

	chartSignalTemplate.getChart().flush();
	chartNoiseEval.getChart().flush();
	chartSignalIn.getChart().flush();
//	chartNoise.getChart().flush();
	chartSignalOut.getChart().flush();

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
