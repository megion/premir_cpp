#include "DigitalFilterTest.h"

void test_filter_charts() {
    graphics::PointChart chartSignalTemplate(710, 460);
    chartSignalTemplate.setWindowTitle("Signal template");
    graphics::ChartThread chartSignalTemplateThread(&chartSignalTemplate);

    graphics::PointChart chartSignalIn(710, 460);
    chartSignalIn.setWindowTitle("Signal+noise");
    graphics::ChartThread chartSignalInThread(&chartSignalIn);

    graphics::PointChart chartNoiseEval(710, 460);
    chartNoiseEval.setWindowTitle("Noise eval");
    graphics::ChartThread chartNoiseEvalThread(&chartNoiseEval);

    graphics::PointChart chartSignalOut(710, 460);
    chartSignalOut.setWindowTitle("Signal out");
    graphics::ChartThread chartSignalOutThread(&chartSignalOut);

    graphics::PointChart chartFilterWeights(710, 460);
    chartFilterWeights.setWindowTitle("Weights");
    graphics::ChartThread chartFilterWeightsThread(&chartFilterWeights);


    double TN = 500.0; // кол-во точек на период
    size_t N = 1200; // размеры
    double sampleStep = 2 * MY_PI / TN;
    double speed = 1.0 / (N * 36.0 * 10.0);

    ml::DigitalFilter df(N, speed, 0.0);
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 0.1);

    for (double i = 0.0; i <= 2 * MY_PI * 10.0; i = i + sampleStep) {
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
            chartSignalTemplate.redrawNewPoints(i, signal);
            chartNoiseEval.redrawNewPoints(i, noiseEval);
            chartSignalIn.redrawNewPoints(i, signalAndNoise);
            chartSignalOut.redrawNewPoints(i, signalEval);

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
            chartFilterWeights.getData()->removeData();
            double k = 0.0;
            for (double &w : (*df.getWeightsArray())) {
                chartFilterWeights.getData()->addPoint(k, w);
                ++k;

            }
            chartFilterWeights.drawBackground();
            chartFilterWeights.drawAxes();
            chartFilterWeights.drawAxesLabels();
            chartFilterWeights.drawPoints();
            chartFilterWeights.flush();
            /////////

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
//			chart.getChart().drawBackground();

        }
    }

}

void digitalFilter_test() {
    suite("charts");
//	mytest(filter_charts);

}
