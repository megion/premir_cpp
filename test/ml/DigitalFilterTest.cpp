#include "DigitalFilterTest.h"

void test_filter_charts() {
    graphics::PointChart chartSignalTemplate(false, 710, 460);
    chartSignalTemplate.setWindowTitle("Signal template");
    graphics::ChartThread<bool> chartSignalTemplateThread(&chartSignalTemplate);

    graphics::PointChart chartSignalIn(false, 710, 460);
    chartSignalIn.setWindowTitle("Signal+noise");
    graphics::ChartThread<bool> chartSignalInThread(&chartSignalIn);

    graphics::PointChart chartNoiseEval(false, 710, 460);
    chartNoiseEval.setWindowTitle("Noise eval");
    graphics::ChartThread<bool> chartNoiseEvalThread(&chartNoiseEval);

    graphics::PointChart chartSignalOut(false, 710, 460);
    chartSignalOut.setWindowTitle("Signal out");
    graphics::ChartThread<bool> chartSignalOutThread(&chartSignalOut);

    graphics::PointChart chartFilterWeights(false, 710, 460);
    chartFilterWeights.setWindowTitle("Weights");
    graphics::ChartThread<bool> chartFilterWeightsThread(&chartFilterWeights);


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
            chartSignalTemplate.redrawNewPoint(i, signal);
            chartNoiseEval.redrawNewPoint(i, noiseEval);
            chartSignalIn.redrawNewPoint(i, signalAndNoise);
            chartSignalOut.redrawNewPoint(i, signalEval);

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
            chartFilterWeights.removeDataSafely();
            utils::CArrayList<graphics::ChartData<bool>::Point> wPoints(df.getWeightsArray()->size());
            for (size_t k = 0; k<df.getWeightsArray()->size(); ++k) {
                graphics::ChartData<bool>::Point wp = {(double)k, (df.getWeightsArray()->getArray())[k]};
                wPoints.push(wp);
            }
            chartFilterWeights.getData()->addPoints(0, wPoints.getArray(), wPoints.size());
            chartFilterWeights.drawOnWindowSafely();
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
