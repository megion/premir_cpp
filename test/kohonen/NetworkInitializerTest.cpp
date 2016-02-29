#include "NetworkInitializerTest.h"

namespace test {
    namespace kohonen_initializer {

        typedef utils::RMatrix<models::NeuronInfo, float> OutCodes;
        typedef typename utils::RMatrix<models::NeuronInfo, float>::Row Neuron;

        void readInitializer(file::CsvFileReader<char> *csvReader) {
            // skip first line
            csvReader->toEndLine();
        }

        bool isSkipSample(char *buffer, size_t bytesRead) {
            if (bytesRead == 0 || buffer[0] == 'x' || buffer[0] == 'X') {
                return true;
            } else {
                return false;
            }
        }

        OutCodes *read_matrix_file(const char *filename,
                                   int skipLines, size_t colSize) {
            file::CsvFileReader<char> reader(filename, ' ');
            OutCodes *somCodesMatrix = new OutCodes(0, colSize);
            // skip to lines
            for (int i = 0; i < skipLines; ++i) {
                reader.toEndLine();
            }

            while (reader.hasNext()) {
                float row[colSize];
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
            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<float> dataReader(&csvReader, readInitializer, isSkipSample, 5,
                                                                     false);

            kohonen::NetworkInitializer<float, float> initializer(&dataReader);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            // TODO: в тестах для генерации кодов нейронов таких же как и в
            // проверочном файле 'som_initialized.cod', необходимо задавать
            // начальное рандомное число setNextValue(1) т.к. файл
            // 'som_initialized.cod' был сгенерирован при начальном значении = 1
            randomEngine->setNextValue(1);
            //randomEngine->initGenerator();

            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *resultsMatrix = initializer.lineInitialization(xdim, ydim, dim);
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
            file::CsvFileReader<char> csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<float> dataReader(&csvReader, readInitializer, isSkipSample, dim,
                                                                     true);

            OutCodes *somCodesMatrix = read_some_initilized_codes();

            kohonen::winner::EuclideanWinnerSearch<float, float> winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction<float> alphaFunc;
            kohonen::mapdist::HexaMapDistance<float> mapDist;

            kohonen::neighadap::BubbleNeighborAdaptation<float, float> neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<float, float> trainer(&alphaFunc, &winnerSearcher, &neiAdap, 0.002, 3.0, xdim, ydim);

            trainer.training(somCodesMatrix, &dataReader, 10000);

            OutCodes *expectedCodesMatrix = read_codes_file(
                    "../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*expectedCodesMatrix, 0.001, true));

            kohonen::SomTrainer<float, float>::QuantumError qe = trainer.quantizationError(somCodesMatrix, &dataReader);

            assert_range(qe.sumWinnerDistance / qe.samplesSize, 4.7287, 0.0001);

            delete expectedCodesMatrix;
            delete somCodesMatrix;
        }

        void test_eucw_gaussian_rect_16_12_som_training() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<float> dataReader(&csvReader, readInitializer, isSkipSample, dim,
                                                                     true);

            OutCodes *somCodesMatrix = read_some_initilized_codes();

            kohonen::winner::EuclideanWinnerSearch<float, float> winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction<float> alphaFunc;
            kohonen::mapdist::RectMapDistance<float> mapDist;

            kohonen::neighadap::GaussianNeighborAdaptation<float, float> neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<float, float> trainer(&alphaFunc, &winnerSearcher, &neiAdap, 0.002, 3, xdim, ydim);

            trainer.training(somCodesMatrix, &dataReader, 10000);

            OutCodes *expectedCodesMatrix = read_codes_file(
                    "../test/datafiles/kohonen/som_trained_10000_eucw_gaussian_rect_16_12.cod", 1);

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*expectedCodesMatrix, 0.001, true));

            delete expectedCodesMatrix;
            delete somCodesMatrix;
        }

        void buildAndShowSammonMap(OutCodes *somTrainedMatrix,
                                   graphics::SammonMapChart<float> &sammonChart) {
            kohonen::SammonMap<float> sammonMap(somTrainedMatrix->getRowSize());
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
                      graphics::UMatChart<float> &chart, size_t xdim, size_t ydim, size_t dim) {
            kohonen::umat::HexaUMat<float> umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();
//            umat.averageUMatrix();

            chart.removeDataSafely();
            chart.addHexaUMatPoints(umat.getUMatrix());
            chart.drawOnWindow();
        }

        void test_eucw_bubble_hexa_16_12_sammon() {
//            size_t xdim = 16;
//            size_t ydim = 12;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<float> dataReader(&csvReader, readInitializer, isSkipSample, dim,
                                                                     true);

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::SammonMap<float> sammonMap(somTrainedMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somTrainedMatrix);
            sammonMap.buildMap(1000);

            sammonMap.getMapPoints();

            OutCodes *expectedSammonMatrix = read_sammon_file("../test/datafiles/kohonen/sammon_out_result.sam", 1);
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
            size_t ydim = 12;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<float> dataReader(&csvReader, readInitializer, isSkipSample, dim,
                                                                     true);

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::SammonMap<float> sammonMap(somTrainedMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somTrainedMatrix);
            sammonMap.buildMap(1000);

            graphics::SammonMapChart<float> sammonChart(xdim, 740, 740);
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

        void test_visible_som_training() {
            size_t xdim = 260;
            size_t ydim = 260;
            size_t dim = 5;

            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader("../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<float> dataReader(&csvReader, readInitializer, isSkipSample, dim,
                                                                     false);

//            utils::SMatrix<float> *somCodesMatrix = read_some_initilized_codes();

            kohonen::NetworkInitializer<float, float> initializer(&dataReader);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            randomEngine->setNextValue(1);
            OutCodes *somCodesMatrix = initializer.lineInitialization(xdim, ydim, dim);

            kohonen::winner::EuclideanWinnerSearch<float, float> winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction<float> alphaFunc;
            kohonen::mapdist::HexaMapDistance<float> mapDist;

            kohonen::neighadap::BubbleNeighborAdaptation<float, float> neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<float, float> trainer(&alphaFunc, &winnerSearcher, &neiAdap, 0.002, 3.0, xdim, ydim);

            //
            graphics::PointChart qErrorChart(710, 460);
            qErrorChart.setWindowTitle("Quantum error");
            graphics::ChartThread<bool> chartThread(&qErrorChart);

            size_t winnerSize = winnerSearcher.getWinnerSize();
            size_t colSize = somCodesMatrix->getColSize();

            size_t teachSize = 20000;
            double qerror = 0;
            int step = 1000;
            int step2 = 6000;

//            graphics::SammonMapChart sammonChart(xdim, 1200, 700);
//            sammonChart.setWindowTitle("Sammon Map");
//            graphics::ChartThread<bool> sammonChartThread(&sammonChart);

//            buildAndShowSammonMap(somCodesMatrix, sammonChart);



            for (size_t le = 0; le < teachSize; ++le) {
                models::DataSample<float> inRow[colSize];
                bool hasInRow = dataReader.readNext(inRow, colSize);
                if (!hasInRow) {
                    // значит достигли конца данных, начинаем читать с начала
                    // установить поток на начало
                    dataReader.rewindReader();
                    // читаем первую стоку данных
                    dataReader.readNext(inRow, colSize);
                }

                kohonen::winner::WinnerInfo<float> winners[winnerSize];
                bool ok = trainer.trainingBySample(somCodesMatrix, inRow,
                                                   winners, teachSize, le);
                if (ok) {
                    int cnt = le % step;
                    qerror += std::sqrt(winners[0].diff);
                    if (cnt == 0 && le != 0) {
                        qerror = qerror / step;
                        qErrorChart.redrawNewPoint(le, qerror);
                        qerror = 0;
//                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    }

                    if (le % step2 == 0) {
//                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
//                        buildAndShowSammonMap(somCodesMatrix, sammonChart);
                    }


                }
            }

            graphics::UMatChart<float> umatChart(1740, 1740);
            umatChart.setWindowTitle("UMat");
            graphics::ChartThread<float> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);

//            graphics::SammonMapChart<float> sammonChart2(xdim, 1200, 700);
//            sammonChart2.setWindowTitle("Sammon Map2");
//            graphics::ChartThread<bool> sammonChartThread2(&sammonChart2);
//            buildAndShowSammonMap(somCodesMatrix, sammonChart2);

            delete somCodesMatrix;
        }

        void test_umatrix_hexa() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            OutCodes *expectedUMatrix =
                    read_matrix_file("../test/datafiles/kohonen/umat_scaled_hexa_16_12.umat", 0, 23);

            kohonen::umat::HexaUMat<float> umat(xdim, ydim, dim);
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

            kohonen::umat::HexaUMat<float> umat(xdim, ydim, dim);
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

            kohonen::umat::HexaUMat<float> umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();

            assert(umat.getUMatrix()->equalsWithError(*expectedUMatrix, 0.001, true));

            delete somTrainedMatrix;
            delete expectedUMatrix;
        }

        void test_umatrix_visible() {
            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            OutCodes *somTrainedMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_10000_eucw_bubble_hexa_16_12.cod", 1);

            kohonen::umat::HexaUMat<float> umat(xdim, ydim, dim);
            umat.initializeMat(somTrainedMatrix);
            umat.buildUMatrix();

            graphics::UMatChart<float> umatChart(500, 740);
            umatChart.setWindowTitle("UMat");
            graphics::ChartThread<float> chartThread(&umatChart);
            umatChart.addHexaUMatPoints(umat.getUMatrix());
            umatChart.drawOnWindow();

            graphics::UMatChart<float> umatAChart(500, 740);
            umatAChart.setWindowTitle("UMatA");
            graphics::ChartThread<float> chartThreadA(&umatAChart);
            umat.averageUMatrix();
            umatAChart.addHexaUMatPoints(umat.getUMatrix());
            umatAChart.drawOnWindow();
            umatAChart.saveImage();

            graphics::UMatChart<float> umatAMChart(500, 740);
            umatAMChart.setWindowTitle("UMatM");
            graphics::ChartThread<float> chartThreadAM(&umatAMChart);
            umat.medianUMatrix();
            umatAMChart.addHexaUMatPoints(umat.getUMatrix());
            umatAMChart.drawOnWindow();

            // нарисуем медианную матрицу заново
//            kohonen::umat::HexaUMat<float> umatM(xdim, ydim, dim);
//            umatM.initializeMat(somTrainedMatrix);
//            umatM.buildUMatrix();
//            umatM.medianUMatrix();
//            graphics::UMatChart<float> umatMChart(500, 740);
//            umatMChart.setWindowTitle("UMatM");
//            graphics::ChartThread<float> chartThreadM(&umatMChart);
//            umatMChart.addHexaUMatPoints(umat.getUMatrix());
//            umatMChart.draw();
        }

        void network_initializer_test() {
            suite("NetworkInitializer_test");
            mytest(line_initialization);
            mytest(eucw_bubble_hexa_16_12_som_training);
            mytest(eucw_gaussian_rect_16_12_som_training);
            mytest(umatrix_hexa);
            mytest(umatrix_hexa_average);
            mytest(umatrix_hexa_median);
            mytest(eucw_bubble_hexa_16_12_sammon);

//            mytest(sammon_visible);
            mytest(umatrix_visible);
//            mytest(visible_som_training);
        }
    }
}



