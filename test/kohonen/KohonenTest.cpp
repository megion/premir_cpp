#include "KohonenTest.h"

namespace test {
    namespace kohonen_test {

        typedef utils::RMatrix<models::NeuronInfo, double> OutCodes;
//        typedef typename utils::RMatrix<models::NeuronInfo, float>::Row Neuron;

        OutCodes *read_matrix_file(const char *filename,
                                   int skipLines, size_t colSize) {
            file::CsvFileReader reader(filename, ' ');
            OutCodes *somCodesMatrix = new OutCodes(0, colSize);
            // skip to lines
            for (int i = 0; i < skipLines; ++i) {
                reader.toEndLine();
            }

            while (reader.hasNext()) {
                double row[colSize];
                size_t lNo1 = reader.getLineNumber();
                for (size_t j = 0; j < colSize; ++j) {
                    reader.read(row[j]);
                }
                size_t lNo2 = reader.getLineNumber();
                if (lNo1 == lNo2) {
                    // TODO: строка прочитана не доконца, тогда прочитать до конца
                    reader.toEndLine();
                }
                if (!reader.isEmptyRead()) {
                    somCodesMatrix->pushRow(row);
                }
            }

            return somCodesMatrix;
        }

        OutCodes *read_codes_file(const char *filename, int skipLines) {
            return read_matrix_file(filename, skipLines, 5);
        }

        OutCodes *read_sammon_file(const char *filename, int skipLines) {
            return read_matrix_file(filename, skipLines, 2);
        }

        OutCodes *read_some_initilized_codes() {
            return read_codes_file("../test/datafiles/kohonen/som_initialized.cod", 2);
        }

        void test_line_initialization() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            KohonenDemoCsvFileRowParser demoRowParser;
            file::stream::CsvFileStreamReader<DemoInRow> dataReader(&csvReader, &demoRowParser, dim, false);
            file::CsvFileSummary<DemoInRow> summary(dim);
            summary.collectSummary(0, &csvReader, &demoRowParser);

            kohonen::NetworkInitializer<DemoInRow> initializer(&dataReader, &summary);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            // TODO: в тестах для генерации кодов нейронов таких же как и в
            // проверочном файле 'som_initialized.cod', необходимо задавать
            // начальное рандомное число setNextValue(1) т.к. файл
            // 'som_initialized.cod' был сгенерирован при начальном значении = 1
            randomEngine->setNextValue(1);
            //randomEngine->initGenerator();

            OutCodes *resultsMatrix = initializer.lineInitialization(xdim, ydim, dim, false);
            OutCodes *somCodesMatrix = read_some_initilized_codes();

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*resultsMatrix, 0.001, true));

            delete somCodesMatrix;
            delete resultsMatrix;
        }

        void test_eucw_bubble_hexa_16_12_som_training() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            KohonenDemoCsvFileRowParser demoRowParser;
            file::stream::CsvFileStreamReader<DemoInRow> dataReader(&csvReader, &demoRowParser, dim, false);

            OutCodes *somCodesMatrix = read_some_initilized_codes();

            kohonen::winner::DefaultWinnerDistance winnerDistance;
            kohonen::winner::EuclideanWinnerSearch winnerSearcher(&winnerDistance);
            kohonen::alphafunc::LinearAlphaFunction alphaFunc;
            kohonen::mapdist::HexaMapDistance mapDist;

            kohonen::neighadap::BubbleNeighborAdaptation neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<DemoInRow> trainer(&alphaFunc, &winnerSearcher, &neiAdap, 0.002, 3.0, xdim, ydim);

            trainer.training(somCodesMatrix, &dataReader, false, nullptr, 10000);

            OutCodes *expectedCodesMatrix = read_codes_file(
                    "../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*expectedCodesMatrix, 0.01, true));

            kohonen::SomTrainer<DemoInRow>::QuantumError qe =
                    trainer.quantizationError(somCodesMatrix, &dataReader, false, nullptr);

            assert_range(qe.sumWinnerDistance / qe.samplesSize, 4.7287, 0.0001);

            delete expectedCodesMatrix;
            delete somCodesMatrix;
        }

        void test_eucw_gaussian_rect_16_12_som_training() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            KohonenDemoCsvFileRowParser demoRowParser;
            file::stream::CsvFileStreamReader<DemoInRow> dataReader(&csvReader, &demoRowParser, dim, true);

            OutCodes *somCodesMatrix = read_some_initilized_codes();

            kohonen::winner::DefaultWinnerDistance winnerDistance;
            kohonen::winner::EuclideanWinnerSearch winnerSearcher(&winnerDistance);
            kohonen::alphafunc::LinearAlphaFunction alphaFunc;
            kohonen::mapdist::RectMapDistance mapDist;

            kohonen::neighadap::GaussianNeighborAdaptation neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<DemoInRow> trainer(&alphaFunc, &winnerSearcher, &neiAdap, 0.002, 3, xdim, ydim);

            trainer.training(somCodesMatrix, &dataReader, false, nullptr, 10000);

            OutCodes *expectedCodesMatrix = read_codes_file(
                    "../test/datafiles/kohonen/som_trained_10000_eucw_gaussian_rect_16_12.cod", 1);

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*expectedCodesMatrix, 0.01, true));

            delete expectedCodesMatrix;
            delete somCodesMatrix;
        }

        void buildAndShowSammonMap(OutCodes *somTrainedMatrix, graphics::SammonMapChart &sammonChart) {
            kohonen::SammonMap sammonMap(somTrainedMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somTrainedMatrix);

            for (size_t i = 0; i < 200; ++i) {
                sammonChart.removeDataSafely();
                sammonMap.doOneIteration();

//                utils::CArrayList<graphics::ChartData<bool>::Point> points(sammonMap.getMapPoints()->size());
//                for (size_t i = 0; i < sammonMap.getMapPoints()->size(); ++i) {
//                    kohonen::SammonMap<float>::Point &r = (*sammonMap.getMapPoints())[i];
//                    graphics::ChartData<bool>::Point p = {r.x, r.y};
//                    points.push(p);
//                }
//                sammonChart.getData()->addPoints(0, points.getArray(), points.size());

                sammonChart.addSammonMapPoints(sammonMap.getMapPoints());
                sammonChart.drawOnWindow();
            }
        }

        void drawUMat(OutCodes *somTrainedMatrix,
                      graphics::UMatChart<char> &chart, size_t xdim, size_t ydim, size_t dim) {
            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();
//            umat.averageUMatrix();

            chart.removeDataSafely();
            chart.addHexaUMatPoints(umat.getUMatrix());
            chart.drawOnWindow();
        }

        void test_eucw_bubble_hexa_16_12_sammon() {
            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::SammonMap sammonMap(somTrainedMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somTrainedMatrix);
            sammonMap.buildMap(1000);

            sammonMap.getMapPoints();

            OutCodes *expectedSammonMatrix = read_sammon_file("../test/datafiles/kohonen/sammon_out_result.sam", 1);
//            sammonMap.getMapPoints()->print("");
            assert(sammonMap.getMapPoints()->size() == expectedSammonMatrix->getRowSize());
            for (size_t r = 0; r < expectedSammonMatrix->getRowSize(); ++r) {
                assert_range((*sammonMap.getMapPoints())[r].x, (*expectedSammonMatrix)[r][0], 0.001);
                assert_range((*sammonMap.getMapPoints())[r].y, (*expectedSammonMatrix)[r][1], 0.001);
            }

            delete somTrainedMatrix;
            delete expectedSammonMatrix;
        }

        void test_sammon_visible() {
            size_t xdim = 16;
            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::SammonMap sammonMap(somTrainedMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somTrainedMatrix);
            sammonMap.buildMap(1000);

            graphics::SammonMapChart sammonChart(xdim, 740, 740);
            sammonChart.setWindowTitle("Sammon Map");
            graphics::ChartThread<bool> chartThread(&sammonChart);
            sammonChart.addSammonMapPoints(sammonMap.getMapPoints());
//            utils::CArrayList<graphics::ChartData<bool>::Point> points(sammonMap.getMapPoints()->size());
//            for (size_t i = 0; i < sammonMap.getMapPoints()->size(); ++i) {
//                kohonen::SammonMap<float>::Point &r = (*sammonMap.getMapPoints())[i];
//                graphics::ChartData<bool>::Point p = {r.x, r.y};
//                points.push(p);
//            }
//            sammonChart.getData()->addPoints(0, points.getArray(), points.size());
            sammonChart.drawOnWindow();

            delete somTrainedMatrix;
        }

        void test_umatrix_hexa() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            OutCodes *expectedUMatrix =
                    read_matrix_file("../test/datafiles/kohonen/umat_scaled_hexa_16_12.umat", 0, 23);

            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();

            assert(umat.getUMatrix()->equalsWithError(*expectedUMatrix, 0.001, true));

            delete somTrainedMatrix;
            delete expectedUMatrix;
        }

        void test_umatrix_hexa_average() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            OutCodes *expectedUMatrix =
                    read_matrix_file("../test/datafiles/kohonen/umat_scaled_hexa_16_12_average.umat", 0, 23);

            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.averageUMatrix();

            assert(umat.getUMatrix()->equalsWithError(*expectedUMatrix, 0.001, true));

            delete somTrainedMatrix;
            delete expectedUMatrix;
        }

        void test_umatrix_hexa_median() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            OutCodes *expectedUMatrix =
                    read_matrix_file("../test/datafiles/kohonen/umat_scaled_hexa_16_12_median.umat", 0, 23);

            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();

            assert(umat.getUMatrix()->equalsWithError(*expectedUMatrix, 0.001, true));

            delete somTrainedMatrix;
            delete expectedUMatrix;
        }

        void test_som_labeling() {
            size_t xdim = 10;
            size_t ydim = 10;

            utils::hash::CharHash cHash;
            kohonen::labeling::SomLabeling<char> somLabeling(xdim, ydim, &cHash);
            char labelA = 'A';
            somLabeling.addWinner(2, labelA);
            somLabeling.addWinner(2, labelA);
            somLabeling.addWinner(3, labelA);
            char labelB = 'B';
            somLabeling.addWinner(2, labelB);
            somLabeling.addWinner(4, labelB);
            somLabeling.addWinner(4, labelB);
            somLabeling.addWinner(4, labelB);

            assert(somLabeling.getWinnerLabels()->getValue(2, 'A')->count == 2);
            assert(somLabeling.getWinnerLabels()->getValue(3, 'A')->count == 1);
            assert(somLabeling.getWinnerLabels()->getValue(2, 'B')->count == 1);
            assert(somLabeling.getWinnerLabels()->getValue(4, 'B')->count == 3);

            somLabeling.collectSummary();
            assert(somLabeling.getSummaryLabels()->getValue(0, 'A')->maxCount == 2);
            assert(somLabeling.getSummaryLabels()->getValue(0, 'A')->minCount == 1);
            assert(somLabeling.getSummaryLabels()->getValue(0, 'B')->maxCount == 3);
            assert(somLabeling.getSummaryLabels()->getValue(0, 'B')->minCount == 1);

            somLabeling.addWinner(4, labelB);
            assert(somLabeling.getSummaryLabels()->getValue(0, 'B')->maxCount == 3);
            somLabeling.collectSummary();
            somLabeling.collectSummary();
            somLabeling.collectSummary();
            assert(somLabeling.getSummaryLabels()->getValue(0, 'B')->maxCount == 4);
        }

        void test_umatrix_visible() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();

            graphics::UMatChart<char> umatChart(500, 740);
            umatChart.setWindowTitle("UMat");
            graphics::ChartThread<graphics::UMatCell> chartThread(&umatChart);
            umatChart.addHexaUMatPoints(umat.getUMatrix());
            umatChart.drawOnWindow();

            graphics::UMatChart<char> umatAChart(500, 740);
            umatAChart.setWindowTitle("UMatA");
            graphics::ChartThread<graphics::UMatCell> chartThreadA(&umatAChart);
            umat.averageUMatrix();
            umatAChart.addHexaUMatPoints(umat.getUMatrix());
            umatAChart.drawOnWindow();

            graphics::UMatChart<char> umatAMChart(500, 740);
            umatAMChart.setWindowTitle("UMatM");
            graphics::ChartThread<graphics::UMatCell> chartThreadAM(&umatAMChart);
            umat.medianUMatrix();
            umatAMChart.addHexaUMatPoints(umat.getUMatrix());
            umatAMChart.drawOnWindow();
//            umatAChart.saveImage("u-mat-m2.png");
        }

        void test_umatrix_labeling() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::umat::HexaUMat umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();
//            umat.averageUMatrix();

            graphics::UMatChart<DemoStringKey> umatMChart(500, 740);
            umatMChart.setWindowTitle("UMatM");
            graphics::ChartThread<graphics::UMatCell> chartThreadAM(&umatMChart);
            umatMChart.addHexaUMatPoints(umat.getUMatrix());
            umatMChart.drawOnWindow();

            file::CsvFileReader csvReader("../test/datafiles/kohonen/ex_fts.dat", ' ');
            DemoFtsCsvFileRowParser demoFtsRowParser;
            file::stream::CsvFileStreamReader<DemoInRowFts> dataReader(&csvReader, &demoFtsRowParser, dim, false);

            // init labeling
            DemoStringKeyHash strHash;
            kohonen::labeling::SomLabeling<DemoStringKey> somLabeling(xdim, ydim, &strHash);

            kohonen::winner::DefaultWinnerDistance winnerDistance;
            kohonen::winner::EuclideanWinnerSearch winnerSearcher(&winnerDistance);
            models::DataSample samples[dim];
            DemoInRowFts rowData;
            size_t winnerSize = winnerSearcher.getWinnerSize();
            // поиск победителя
            std::cout << std::endl;
            while (dataReader.readNext(rowData, samples)) {
                kohonen::winner::WinnerInfo winners[winnerSize];
                bool ok = winnerSearcher.search(somTrainedMatrix, samples, winners);
                if (ok) {
                    std::cout << " " << winners[0].index;
                    somLabeling.addWinner(winners[0].index, rowData.ftsLabel);
                }
            }
            std::cout << std::endl;

            DemoStringKey faultKey;
            faultKey.setLabel("fault");
            somLabeling.collectSummary();
//            umatMChart.setLabelsForKey(somLabeling.getWinnerLabels(), faultKey, xdim, ydim, -1.0);
            umatMChart.setAllLabels(somLabeling.getWinnerLabels(), xdim, ydim, nullptr, -1.0);
            umatMChart.drawOnWindow();

            delete somTrainedMatrix;
        }

        void test_visible_som_training() {
            size_t xdim = 80;
            size_t ydim = 80;
            size_t dim = 20;
            size_t teachSize = 800000;
            bool isScale = true;

            // инициализация потока чтения файла с данными
            file::CsvFileReader csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            KohonenDemo2CsvFileRowParser demoRowParser;
            file::stream::CsvFileStreamReader<DemoInRow> dataReader(&csvReader, &demoRowParser, dim, false);
            file::CsvFileSummary<DemoInRow> summary(dim);
            summary.collectSummary(0, &csvReader, &demoRowParser); // 0 - значит без ограничений
//            summary.getSummary()->print();

            kohonen::NetworkInitializer<DemoInRow> initializer(&dataReader, &summary);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            randomEngine->setNextValue(1);
            OutCodes *somCodesMatrix = initializer.lineInitialization(xdim, ydim, dim, isScale);

            kohonen::winner::DefaultWinnerDistance winnerDistance;
            kohonen::winner::EuclideanWinnerSearch winnerSearcher(&winnerDistance);
            kohonen::alphafunc::LinearAlphaFunction alphaFunc;
            kohonen::mapdist::HexaMapDistance mapDist;

            kohonen::neighadap::GaussianNeighborAdaptation gausAdap(&mapDist, xdim, ydim);
            kohonen::neighadap::BubbleNeighborAdaptation neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<DemoInRow> trainer(&alphaFunc, &winnerSearcher, &gausAdap, 0.002, 2.0, xdim, ydim);

            //
            graphics::PointChart qErrorChart(true, 710, 460);
            qErrorChart.setWindowTitle("Quantum error");
            graphics::ChartThread<bool> chartThread(&qErrorChart);

            size_t winnerSize = winnerSearcher.getWinnerSize();
            size_t colSize = somCodesMatrix->getColSize();


            double qerror = 0;
            int step = 10000;
            int step2 = 6000;

            graphics::SammonMapChart sammonChart(xdim, 1200, 700);
            sammonChart.setWindowTitle("Sammon Map");
            graphics::ChartThread<bool> sammonChartThread(&sammonChart);
            buildAndShowSammonMap(somCodesMatrix, sammonChart);

            graphics::UMatChart<char> umatChart(700, 700);
            umatChart.setWindowTitle("UMat");
            graphics::ChartThread<graphics::UMatCell> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);

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
                    int cnt = le % step;
                    qerror += std::sqrt(winners[0].diff);
                    if (cnt == 0 && le != 0) {
                        qerror = qerror / step;
                        qErrorChart.redrawNewPoint(le, qerror);
                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
                        qerror = 0;
//                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    }

                    if (le % step2 == 0) {
//                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
//                        buildAndShowSammonMap(somCodesMatrix, sammonChart);
                    }

                }
            }

            graphics::UMatChart<char> umatChart2(4000, 4000);
            umatChart2.setWindowTitle("UMat2");
            graphics::ChartThread<graphics::UMatCell> umchartThread2(&umatChart2);
            drawUMat(somCodesMatrix, umatChart2, xdim, ydim, dim);
            umatChart2.saveImage("u-matrix2.png");

//            graphics::SammonMapChart<float> sammonChart2(xdim, 1200, 700);
//            sammonChart2.setWindowTitle("Sammon Map2");
//            graphics::ChartThread<bool> sammonChartThread2(&sammonChart2);
//            buildAndShowSammonMap(somCodesMatrix, sammonChart2);

            delete somCodesMatrix;
        }

        void kohonen_test() {
            suite("Kohonen");
            mytest(line_initialization);
            mytest(eucw_bubble_hexa_16_12_som_training);
            mytest(eucw_gaussian_rect_16_12_som_training);
            mytest(umatrix_hexa);
            mytest(umatrix_hexa_average);
            mytest(umatrix_hexa_median);
//            mytest(eucw_bubble_hexa_16_12_sammon);
            mytest(som_labeling);

//            mytest(sammon_visible);
//            mytest(umatrix_visible);
//            mytest(umatrix_labeling);
//            mytest(visible_som_training);
        }
    }
}



