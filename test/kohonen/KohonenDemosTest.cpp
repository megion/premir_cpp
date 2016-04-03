#include "KohonenDemosTest.h"

namespace test {
    namespace kohonen_demos_test {

        typedef utils::RMatrix<models::NeuronInfo, double> OutCodes;

        void buildAndShowSammonMap(OutCodes *somTrainedMatrix, graphics::SammonMapChart &sammonChart) {
            kohonen::SammonMap sammonMap(somTrainedMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somTrainedMatrix);

            for (size_t i = 0; i < 200; ++i) {
                sammonChart.removeDataSafely();
                sammonMap.doOneIteration();
                sammonChart.addSammonMapPoints(sammonMap.getMapPoints());
                sammonChart.drawOnWindow();
            }
        }

        void drawUMat(OutCodes *somTrainedMatrix,
                      kohonen::labeling::SomLabeling<char> &somLabeling,
                      graphics::UMatChart<char> &chart, size_t xdim, size_t ydim, size_t dim, double labelThreshold) {
            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();
//            umat.averageUMatrix();

            chart.removeDataSafely();

            somLabeling.collectSummary();
            chart.addHexaUMatPoints(umat.getUMatrix());
            graphics::Color colorA(65535, 0, 0);
            graphics::Color colorI(0, 65535, 0);
            graphics::Color colorN(0, 0, 65535);
            chart.setLabelsForKey(somLabeling.getWinnerLabels(), 'r', xdim, ydim, colorA, labelThreshold);
            chart.setLabelsForKey(somLabeling.getWinnerLabels(), 'd', xdim, ydim, colorI, labelThreshold);
            chart.setLabelsForKey(somLabeling.getWinnerLabels(), 'N', xdim, ydim, colorN, labelThreshold);
            SpeechLabelColorMapper mapper;
//            chart.setAllLabels(somLabeling.getWinnerLabels(), xdim, ydim, &mapper, labelThreshold);
//            chart.setUMatWinnerLabelsForKey(somLabeling.getWinnerLabels(), '#');
//            chart.setUMatWinnerLabelsForKey(somLabeling.getWinnerLabels(), 'O');
            chart.drawOnWindow();
        }

        void test_speech_signal() {
            size_t xdim = 100;
            size_t ydim = 100;
            size_t dim = 20;
            size_t teachSize = 80000;
            double radius = 3.0;
            double alpha = 0.002;
            bool isScale = false;
            double labelThreshold = -0.5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/kohonen/ex1.dat", ' ');
            KohonenDemo2CsvFileRowParser demoRowParser;
            file::stream::CsvFileStreamReader<DemoInRow> dataReader(&csvReader, &demoRowParser);
            file::CsvFileSummary<DemoInRow> summary(dim);
//            summary.collectSummary(0, &csvReader, &demoRowParser); // 0 - значит без ограничений
//            summary.getSummary()->print();

            // TODO: test summary save
//            summary.writeSummary("test_speech_signal_summary.cod");
            summary.readSummary("test_speech_signal_summary.cod");

            kohonen::NetworkInitializer<DemoInRow> initializer(&dataReader, &summary);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            randomEngine->setNextValue(1);
            OutCodes *somCodesMatrix = initializer.lineInitialization(xdim, ydim, dim, isScale);
            kohonen::SomKeeper somKeeper;
            somKeeper.saveSom(somCodesMatrix, "speech_som_initialized_2.cod");

            kohonen::winner::EuclideanWinnerSearch winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction alphaFunc;
            kohonen::mapdist::HexaMapDistance mapDist;

            kohonen::neighadap::GaussianNeighborAdaptation gausAdap(&mapDist, xdim, ydim);
            kohonen::neighadap::BubbleNeighborAdaptation neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<DemoInRow> trainer(&alphaFunc, &winnerSearcher, &neiAdap, alpha, radius, xdim, ydim);

            // init labeling
            utils::hash::CharHash cHash;
            kohonen::labeling::SomLabeling<char> somLabeling(xdim, ydim, &cHash);

            //
            graphics::PointChart qErrorChart(true, 710, 460);
            qErrorChart.setWindowTitle("Quantum error");
            graphics::ChartThread<bool> chartThread(&qErrorChart);

            size_t winnerSize = winnerSearcher.getWinnerSize();
            size_t colSize = somCodesMatrix->getColSize();


            double qerror = 0;
            int step = 1000;
//            int step2 = 6000;

//            graphics::SammonMapChart<float> sammonChart(xdim, 1200, 700);
//            sammonChart.setWindowTitle("Sammon Map");
//            graphics::ChartThread<bool> sammonChartThread(&sammonChart);
//            buildAndShowSammonMap(somCodesMatrix, sammonChart);

            graphics::UMatChart<char> umatChart(800, 800);
            umatChart.setWindowTitle("UMat");
            graphics::ChartThread<graphics::UMatCell> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, somLabeling, umatChart, xdim, ydim, dim, labelThreshold);

            for (size_t le = 0; le < teachSize; ++le) {
                models::DataSample samples[colSize];
                DemoInRow rowData;
                bool hasInRow = dataReader.readNext(rowData, samples);
                if (!hasInRow) {
                    drawUMat(somCodesMatrix, somLabeling, umatChart, xdim, ydim, dim, labelThreshold);
                    // clean drawed data for labels
                    somLabeling.cleanWinnerLabels();

                    // значит достигли конца данных, начинаем читать с начала
                    // установить поток на начало
                    dataReader.rewindReader();
                    // читаем первую стоку данных
                    dataReader.readNext(rowData, samples);
                }

                // нормализация
                if (isScale) {
                    summary.scaleSamples(samples);
                }

//                std::cout << "le: " << le << " " << rowData.label << std::endl;
                kohonen::winner::WinnerInfo winners[winnerSize];
                bool ok = trainer.trainingBySample(somCodesMatrix, samples, winners, teachSize, le);
                if (ok) {
//                    somLabeling.addWinner(0, rowData.label);
//                    char lb = 'A';
//                    size_t wi = le%(xdim*ydim);
//                    somLabeling.addWinner(wi, lb);
                    somLabeling.addWinner(winners[0].index, rowData.label);
//                    int cnt = le % step;
                    qerror += std::sqrt(winners[0].diff);
                    if (le % step==0 && le!=0) {
                        qerror = qerror / step;
                        qErrorChart.redrawNewPoint(le, qerror);
//                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
                        qerror = 0;
//                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    }

//                    if (le % step2 == 0) {
//                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
//                        buildAndShowSammonMap(somCodesMatrix, sammonChart);
//                    }

                }
            }

//            graphics::UMatChart<float, char> umatChart2(2000, 2000);
//            umatChart2.setWindowTitle("UMat2");
//            graphics::ChartThread<graphics::UMatCell<float>> umchartThread2(&umatChart2);
//            drawUMat(somCodesMatrix, somLabeling, umatChart2, xdim, ydim, dim, labelThreshold);
//            umatChart2.saveImage("u-matrix-speech-final.png");

//            graphics::SammonMapChart<float> sammonChart2(xdim, 1200, 700);
//            sammonChart2.setWindowTitle("Sammon Map2");
//            graphics::ChartThread<bool> sammonChartThread2(&sammonChart2);
//            buildAndShowSammonMap(somCodesMatrix, sammonChart2);

            somKeeper.saveSom(somCodesMatrix, "speech_som_trained.cod");

            delete somCodesMatrix;
        }

        void test_house_votes() {
            size_t xdim = 60;
            size_t ydim = 60;
            size_t dim = 16;
            size_t teachSize = 10000;
            int step = 1000;
            double radius = 6.0;
            double alpha = 0.002;
            bool isScale = false;

            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/kohonen/house-votes-84.data.txt", ',');
            HouseVotesCsvFileRowParser rowParser;
            file::stream::CsvFileStreamReader<DemoInRow> dataReader(&csvReader, &rowParser);
            file::CsvFileSummary<DemoInRow> summary(dim);
            summary.collectSummary(0, &csvReader, &rowParser); // 0 - значит без ограничений

            kohonen::NetworkInitializer<DemoInRow> initializer(&dataReader, &summary);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            randomEngine->setNextValue(1);
            OutCodes *somCodesMatrix = initializer.lineInitialization(xdim, ydim, dim, isScale);

            kohonen::winner::EuclideanWinnerSearch winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction alphaFunc;
            kohonen::mapdist::HexaMapDistance mapDist;

            kohonen::neighadap::GaussianNeighborAdaptation gausAdap(&mapDist, xdim, ydim);
            kohonen::neighadap::BubbleNeighborAdaptation neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<DemoInRow> trainer(&alphaFunc, &winnerSearcher, &gausAdap, alpha, radius, xdim, ydim);

            // init labeling
            utils::hash::CharHash cHash;
            kohonen::labeling::SomLabeling<char> somLabeling(xdim, ydim, &cHash);

            //
            graphics::PointChart qErrorChart(true, 710, 460);
            qErrorChart.setWindowTitle("Quantum error");
            graphics::ChartThread<bool> chartThread(&qErrorChart);

            size_t winnerSize = winnerSearcher.getWinnerSize();
            size_t colSize = somCodesMatrix->getColSize();


            double qerror = 0;

//            int step2 = 6000;

//            graphics::SammonMapChart<float> sammonChart(xdim, 1200, 700);
//            sammonChart.setWindowTitle("Sammon Map");
//            graphics::ChartThread<bool> sammonChartThread(&sammonChart);
//            buildAndShowSammonMap(somCodesMatrix, sammonChart);

            graphics::CubehelixCellColorMapper cellColor(200, 0.5, -1.5, 2.0, 2.0);
            graphics::UMatChart<char> umatChart(733, 733, &cellColor);
            umatChart.setWindowTitle("UMat");
            graphics::ChartThread<graphics::UMatCell> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, somLabeling, umatChart, xdim, ydim, dim, -1.0);

            for (size_t le = 0; le < teachSize; ++le) {
                models::DataSample samples[colSize];
                DemoInRow rowData;
                bool hasInRow = dataReader.readNext(rowData, samples);
                if (!hasInRow) {
                    // значит достигли конца данных, начинаем читать с начала
                    // установить поток на начало
                    dataReader.rewindReader();
                    // читаем первую стоку данных
                    dataReader.readNext(rowData, samples);
                }

                // нормализация
                if (isScale) {
                    summary.scaleSamples(samples);
                }

                kohonen::winner::WinnerInfo winners[winnerSize];
                bool ok = trainer.trainingBySample(somCodesMatrix, samples, winners, teachSize, le);
                if (ok) {
                    somLabeling.addWinner(winners[0].index, rowData.label);
//                    int cnt = le % step;
                    qerror += std::sqrt(winners[0].diff);
                    if (le % step==0 && le!=0) {
                        qerror = qerror / step;
                        qErrorChart.redrawNewPoint(le, qerror);
                        drawUMat(somCodesMatrix, somLabeling, umatChart, xdim, ydim, dim, -1.0);
                        // clean drawed data for labels
//                    somLabeling.cleanWinnerLabels();
                        qerror = 0;
                    }

//                    if (le % step2 == 0) {
//                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
//                        buildAndShowSammonMap(somCodesMatrix, sammonChart);
//                    }

                }
            }



//            graphics::UMatChart<float, char> umatChart2(2000, 2000);
//            umatChart2.setWindowTitle("UMat2");
//            graphics::ChartThread<graphics::UMatCell<float>> umchartThread2(&umatChart2);
//            drawUMat(somCodesMatrix, somLabeling, umatChart2, xdim, ydim, dim, labelThreshold);
//            umatChart2.saveImage("u-matrix-speech-final.png");

//            graphics::SammonMapChart<float> sammonChart2(xdim, 1200, 700);
//            sammonChart2.setWindowTitle("Sammon Map2");
//            graphics::ChartThread<bool> sammonChartThread2(&sammonChart2);
//            buildAndShowSammonMap(somCodesMatrix, sammonChart2);

            delete somCodesMatrix;
        }

        void kohonen_demos_test() {
            suite("KohonenDemos");
//            mytest(speech_signal);
//            mytest(house_votes);
        }
    }
}



