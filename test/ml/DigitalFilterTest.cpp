#include "DigitalFilterTest.h"

namespace test {
    namespace ml_test {

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
//                double noise = distribution(generator)
//                               + 2.0 * cos(8.0 * i * PI / 180.0);
                double noise = 2.0 * cos(4.0 * i);


                double signal = 4.0 * cos(i);

                double signalAndNoise = signal + noise;

                if (!std::isnan(signalAndNoise)) {

                    df.addInput(noise);
                    double noiseEval = df.calculateFilterOutSum(); // оценка шума
                    double signalEval = df.updateFilterWeightsByLeastSquaresAlgorithm(
                            signalAndNoise, noiseEval);

                    chartSignalTemplate.redrawNewPoint(0, i, signal);
                    chartNoiseEval.redrawNewPoint(0, i, noiseEval);
                    chartSignalIn.redrawNewPoint(0, i, signalAndNoise);
                    chartSignalOut.redrawNewPoint(0, i, signalEval);

                    ///////// draw filter weight
                    chartFilterWeights.removeDataSafely();
                    utils::CArrayList<graphics::ChartData<bool>::Point> wPoints(df.getWeightsArray()->size());
                    for (size_t k = 0; k < df.getWeightsArray()->size(); ++k) {
                        graphics::ChartData<bool>::Point wp = {(double) k, (df.getWeightsArray()->getArray())[k]};
                        wPoints.push(wp);
                    }
                    chartFilterWeights.getData()->addPoints(0, wPoints.getArray(), wPoints.size());
                    chartFilterWeights.drawOnWindowSafely();
                    chartFilterWeights.flush();
                    /////////

                    std::this_thread::sleep_for(std::chrono::milliseconds(20));

                }
            }

        }


        void digital_filter_test() {
            suite("Digital filter charts");
//            mytest(filter_charts);

        }

        void decision_tree_test() {
            suite("Decision tree");

            utils::hash::CharHash shash;
            shash.setIndexSize(6);

            graphics::PointChart entropyChart(true, 710, 460);
            entropyChart.setWindowTitle("Entropy");
            graphics::ChartThread<bool> entropyChartThread(&entropyChart);

            graphics::Color color1(65535, 0, 0);
            entropyChart.pushSeriesColor(color1);
            graphics::Color color2(0, 65535, 0);
            entropyChart.pushSeriesColor(color2);

            for (int i = 0; i < 100; i++) {
                ml::Entropy<char> ep(&shash);
                for (int k = 0; k < 100; k++) {
                    if (k > i) {
                        ep.addValue('A');
                    } else {
                        ep.addValue('B');
                    }
                }

                double v = ep.calculateEntropy();
                entropyChart.redrawNewPoint(0, i, v);

                double v2 = ep.calculateShenonEntropy();
                entropyChart.redrawNewPoint(1, i, v2);
            }

            entropyChart.saveImage("etropy.png");
        }
    }
}



