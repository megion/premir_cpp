#include "SspyTest.h"

namespace test {
    namespace sspy {

        typedef utils::RMatrix<models::NeuronInfo, double> OutCodes;

        /**
         * return interval of time (uses time.h)
         */
        double get_time() {
            struct timeval timev;
            gettimeofday(&timev, NULL);
            return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
        }

        OutCodes *read_matrix_file(const char *filename, int skipLines, size_t colSize) {
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

        void save_part_of_file(const char *srcFilename, const char *destFilename, size_t lineNumber) {
            file::LineFileReader reader(srcFilename);
            file::CsvFileWriter writer(destFilename);

            file::LineFileReader::LineBuffer buf;
            while (reader.readNextLine(&buf, true) && reader.getLineNumber() <= lineNumber) {
                writer.write(buf.buffer);
                std::free(buf.buffer);
                writer.endLine();
            }
        }

        void test_read_sspy_data_file_by_line() {
            file::LineFileReader lineReader(BIG_DATA_FILE_PATH);

            file::LineFileReader::LineBuffer buf;
            while (lineReader.readNextLine(&buf, true) && lineReader.getLineNumber() <= 3) {
                std::cout << buf.buffer << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
                std::free(buf.buffer);
            }

//            lineReader.calculateTotalLineNumber();

            std::cout << "total number: " << lineReader.getLineNumber() <<
            std::endl;

        }

        void test_csv_parse_sspy_data_file() {
            // sudo mount -t vboxsf izadorozhny ~/share
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            while (reader.hasNext() && reader.getLineNumber() < 10) {
                SspyData row;
                reader.read(row.counter);
                reader.read(row.msg_type);
                reader.read(row.stream_type);
                reader.read(row.date, 11);
                reader.read(row.time, 13);
                reader.read(row.interval, 10);
                reader.toEndLine();
                if (!reader.isEmptyRead()) {
                    std::cout << "lineNumber: " << reader.getLineNumber() <<
                    " counter: " << row.counter << " msg_type: " <<
                    row.msg_type << " stream_type: " << row.stream_type <<
                    " date: " << row.date << " time: " << row.time <<
                    " interval: " << row.interval << std::endl;
                }
            }
        }

        kohonen::SammonMap<double>* buildAndShowSammonMap(OutCodes *somMatrix,
                                   graphics::SammonMapChart<double> &sammonChart, size_t numRepeat) {
            kohonen::SammonMap<double>* sammonMap = new kohonen::SammonMap<double>(somMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap->getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap->initializeMap(somMatrix);

            for (size_t i = 0; i < numRepeat; ++i) {
                sammonChart.removeDataSafely();
                double e = sammonMap->doOneIteration();
                std::cout << "Mapping error " << e << std::endl;
                sammonChart.addSammonMapPoints(sammonMap->getMapPoints());
                sammonChart.drawOnWindow();
            }
            return sammonMap;
        }

        void drawUMat(OutCodes *somMatrix,
                      graphics::UMatChart<double, char> &chart, size_t xdim, size_t ydim,
                      size_t dim) {
            kohonen::umat::HexaUMat<double> umat(xdim, ydim, dim);
            umat.initializeMat(somMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();
//            umat.averageUMatrix();

            chart.removeDataSafely();
            chart.addHexaUMatPoints(umat.getUMatrix());
            chart.drawOnWindow();
        }

        void drawUMatWithLabels(OutCodes *somMatrix,
                      kohonen::labeling::SomLabeling<char> &somLabeling,
                      graphics::UMatChart<double, char> &chart, size_t xdim, size_t ydim,
                      size_t dim) {
            kohonen::umat::HexaUMat<double> umat(xdim, ydim, dim);
            umat.initializeMat(somMatrix);
            umat.buildUMatrix();
            umat.medianUMatrix();
//            umat.averageUMatrix();

            chart.removeDataSafely();
            chart.addHexaUMatPoints(umat.getUMatrix());

            somLabeling.collectSummary();
//            somLabeling.getWinnerLabels()->getMatrix()->print(false);
            chart.addHexaUMatPoints(umat.getUMatrix());

            MsgTypeLabelColorMapper mapper;
            chart.setAllLabels(somLabeling.getWinnerLabels(), xdim, ydim, &mapper, -1.0);
            chart.drawOnWindow();
        }

        void test_collect_summary_sspy_data_file() {
            double start = get_time();
            size_t dim = 17;
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            SspyRowParser rowParser;
            file::stream::CsvFileStreamReader<SspyData, double> dataReader(&reader, &rowParser);
            file::CsvFileSummary<SspyData, double> summary(dim);
            summary.collectSummary(0, &reader, &rowParser); // 0 - значит без ограничений
//            summary.getSummary()->print();

            // TODO: summary save
            summary.writeSummary("sspy_data_summary_6.cod");
            double summaryTime = get_time() - start;
            printf("Summary time: %f\n", summaryTime / 60.0);
        }

        void test_initialization_codes_sspy() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
            size_t ydim = 80;
            bool isScale = true;

            file::CsvFileSummary<SspyData, double> summary(dim);
            summary.readSummary("sspy_data_summary_6.cod");
            summary.getSummary()->print();

            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            SspyRowParser rowParser;
            file::stream::CsvFileStreamReader<SspyData, double> dataReader(&reader, &rowParser);
            kohonen::NetworkInitializer<SspyData, double, double> initializer(&dataReader, &summary);
            kohonen::RandomGenerator *randomEngine = initializer.getRandomGenerator();
            randomEngine->setNextValue(1);
            OutCodes *somCodesMatrix = initializer.lineInitialization(xdim, ydim, dim, isScale);
//            somCodesMatrix->print();

            kohonen::SomKeeper<double> somKeeper;
            file::CsvFileWriter somInitOutFile("sspy_som_initialized_5_80_80.cod");
            somKeeper.saveSom(somCodesMatrix, &somInitOutFile);
            somInitOutFile.close();

            delete somCodesMatrix;

            double summaryTime = get_time() - start;
            printf("Initialization time: %f\n", summaryTime / 60.0);
        }

        void test_visible_initialized_codes_sspy() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
            size_t ydim = 80;

            OutCodes *somCodesMatrix = read_matrix_file("sspy_som_initialized_5_80_80.cod", 0, dim);

            graphics::UMatChart<double, char> umatChart(1200, 700);
            umatChart.setWindowTitle("U-Matrix");
            graphics::ChartThread<graphics::UMatCell<double>> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
            umatChart.saveImage("sspy_u_matrix_80_80_after_init.png");

            graphics::SammonMapChart<double> sammonChart(xdim, 1200, 700);
            sammonChart.setWindowTitle("Sammon map for initialized codes");
            graphics::ChartThread<bool> sammonChartThread(&sammonChart);
            kohonen::SammonMap<double>* sammonMap = buildAndShowSammonMap(somCodesMatrix, sammonChart, 100);

            graphics::SammonMapChart<double> sammonChartImg(xdim, 8000, 8000);
            sammonChartImg.setWindowTitle("Sammon map for initialized codes img");
            graphics::ChartThread<bool> sammonChartThread2(&sammonChartImg);
            sammonChart.addSammonMapPoints(sammonMap->getMapPoints());
            sammonChart.saveImage("sspy_sammon_map_80_80_after_init.png");

            delete somCodesMatrix;
            delete sammonMap;

            double summaryTime = get_time() - start;
            printf("Visualization initialized codes time: %f\n", summaryTime / 60.0);
        }

        void test_trained_codes_sspy() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
            size_t ydim = 80;
            size_t fileRowCount = 126271598;
            size_t teachSize = fileRowCount;
//            size_t teachSize = 2000000;
            double radius = 3.0;
            double alpha = 0.002;
            bool isScale = true;

            double qerror = 0;
            int stepError = 40000;
            int stepUmat = 400000;

            // 1. read initialized codes
            OutCodes *somCodesMatrix = read_matrix_file("sspy_som_initialized_5_80_80.cod", 0, dim);

            // 2. init big data file reader
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            SspyRowParser rowParser;
            file::stream::CsvFileStreamReader<SspyData, double> dataReader(&reader, &rowParser);

            // 3. create Umat chart
            graphics::UMatChart<double, char> umatChart(1200, 700);
            umatChart.setWindowTitle("U-Matrix");
            graphics::ChartThread<graphics::UMatCell<double>> umchartThread(&umatChart);

            // 4. create Quantum error chart
            graphics::PointChart qErrorChart(true, 1200, 700);
            qErrorChart.setWindowTitle("Quantum error");
            graphics::ChartThread<bool> chartThread(&qErrorChart);

            // 5. create trainer
            kohonen::winner::EuclideanWinnerSearch<double, double> winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction<double> alphaFunc;
            kohonen::mapdist::HexaMapDistance<double> mapDist;
//            kohonen::neighadap::GaussianNeighborAdaptation<double, double> gausAdap(&mapDist, xdim, ydim);
            kohonen::neighadap::BubbleNeighborAdaptation<double, double> neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<SspyData, double, double> trainer(&alphaFunc, &winnerSearcher, &neiAdap, alpha, radius,
                                                                 xdim, ydim);

            size_t winnerSize = winnerSearcher.getWinnerSize();
//            size_t colSize = somCodesMatrix->getColSize();

            // load summary
            file::CsvFileSummary<SspyData, double> summary(dim);
            summary.readSummary("sspy_data_summary_6.cod");

            for (size_t le = 0; le < teachSize; ++le) {
                models::DataSample<double> samples[dim];
                SspyData rowData;
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

                kohonen::winner::WinnerInfo<double> winners[winnerSize];
                bool ok = trainer.trainingBySample(somCodesMatrix, samples, winners, teachSize, le);
                if (ok) {
                    qerror += std::sqrt(winners[0].diff);
                    if (le % stepError==0 && le!=0) {
                        qerror = qerror / stepError;
                        qErrorChart.redrawNewPoint(le, qerror);
                        qerror = 0;
                        std::cout<<".";
                    }

                    if (le % stepUmat == 0) {
                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
                        std::cout<<std::endl<<le<<std::endl;
                    }

                }
            }

            kohonen::SomKeeper<double> somKeeper;
            file::CsvFileWriter trainedCodesOutFile("sspy_som_trained_2_80_80.cod");
            somKeeper.saveSom(somCodesMatrix, &trainedCodesOutFile);
            trainedCodesOutFile.close();

            delete somCodesMatrix;

            double summaryTime = get_time() - start;
            printf("Training time: %f\n", summaryTime / 60.0);
        }

        void test_trained_codes_sspy2() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
            size_t ydim = 80;
            size_t fileRowCount = 126271598;

//            size_t teachSize = 2000000;
            double radius = 3.0;
            double alpha = 0.002;
            bool isScale = true;

            double qerror = 0;
            int stepError = 40000;
            int stepUmat = 40000;
            int stepRepeat = 400000;

            size_t teachSize = 10*stepRepeat;

            // 1. read initialized codes
            OutCodes *somCodesMatrix = read_matrix_file("sspy_som_initialized_5_80_80.cod", 0, dim);

            // 2. init big data file reader
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            SspyRowParser rowParser;
            file::stream::CsvFileStreamReader<SspyData, double> dataReader(&reader, &rowParser);

            // 3. create Umat chart
            graphics::CubehelixCellColorMapper cellColor(200, 0.5, -1.5, 2.0, 2.0);
            graphics::UMatChart<double, char> umatChart(1200, 700, &cellColor);
            umatChart.setWindowTitle("U-Matrix");
            graphics::ChartThread<graphics::UMatCell<double>> umchartThread(&umatChart);

            // 4. create Quantum error chart
            graphics::PointChart qErrorChart(true, 1200, 700);
            qErrorChart.setWindowTitle("Quantum error");
            graphics::ChartThread<bool> chartThread(&qErrorChart);

            // 5. create trainer
            kohonen::winner::EuclideanWinnerSearch<double, double> winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction<double> alphaFunc;
            kohonen::mapdist::HexaMapDistance<double> mapDist;
            kohonen::neighadap::GaussianNeighborAdaptation<double, double> gausAdap(&mapDist, xdim, ydim);
            kohonen::neighadap::BubbleNeighborAdaptation<double, double> neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<SspyData, double, double> trainer(&alphaFunc, &winnerSearcher, &gausAdap, alpha, radius,
                                                                  xdim, ydim);

            size_t winnerSize = winnerSearcher.getWinnerSize();
//            size_t colSize = somCodesMatrix->getColSize();

            // load summary
            file::CsvFileSummary<SspyData, double> summary(dim);
            summary.readSummary("sspy_data_summary_6.cod");

            for (size_t le = 0; le < teachSize; ++le) {
                // через каждый шаг stepRepeat устанавливаем чтение сначала для того чтобы обычатся на первых stepRepeat
                // строках
                if (stepRepeat!=0 && le%stepRepeat==0) {
                    dataReader.rewindReader();
                }
                models::DataSample<double> samples[dim];
                SspyData rowData;
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

                kohonen::winner::WinnerInfo<double> winners[winnerSize];
                bool ok = trainer.trainingBySample(somCodesMatrix, samples, winners, teachSize, le);
                if (ok) {
                    qerror += std::sqrt(winners[0].diff);
                    if (le % stepError==0 && le!=0) {
                        qerror = qerror / stepError;
                        qErrorChart.redrawNewPoint(le, qerror);
                        qerror = 0;
                        std::cout<<".";
                    }

                    if (le % stepUmat == 0) {
                        drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);
                        std::cout<<std::endl<<le<<std::endl;
                    }

                }
            }

            kohonen::SomKeeper<double> somKeeper;
            file::CsvFileWriter trainedCodesOutFile("sspy_som_trained_80_80_for_400000_by_10_gaus.cod");
            somKeeper.saveSom(somCodesMatrix, &trainedCodesOutFile);
            trainedCodesOutFile.close();

            delete somCodesMatrix;

            double summaryTime = get_time() - start;
            printf("Training time: %f\n", summaryTime / 60.0);
        }

        void test_visible_trained_codes_sspy() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
            size_t ydim = 80;

            OutCodes *somCodesMatrix = read_matrix_file("sspy_som_trained_1_80_80.cod", 0, dim);

//            graphics::CubehelixCellColorMapper cellColor(200, 0.5, -1.5, 2.0, 2.0);
            graphics::UMatChart<double, char> umatChart(733, 733);

            umatChart.setWindowTitle("U-Matrix");
            graphics::ChartThread<graphics::UMatCell<double>> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);

            umatChart.saveImage("sspy_u_matrix_80_80_after_trained.png");

//            graphics::SammonMapChart<double> sammonChart(xdim, 733, 733);
//            sammonChart.setWindowTitle("Sammon map for trained codes");
//            graphics::ChartThread<bool> sammonChartThread(&sammonChart);
//            kohonen::SammonMap<double>* sammonMap = buildAndShowSammonMap(somCodesMatrix, sammonChart, 100);
//
//            graphics::SammonMapChart<double> sammonChartImg(xdim, 8000, 8000);
//            sammonChartImg.setWindowTitle("Sammon map for initialized codes img");
//            graphics::ChartThread<bool> sammonChartThread2(&sammonChartImg);
//            sammonChartImg.addSammonMapPoints(sammonMap->getMapPoints());
//            sammonChartImg.saveImage("sspy_sammon_map_80_80_after_trained_img2.png");

            delete somCodesMatrix;
//            delete sammonMap;

            double summaryTime = get_time() - start;
            printf("Visualization trained codes time: %f\n", summaryTime / 60.0);
        }

        void test_visible_labling_trained_codes_sspy() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
            size_t ydim = 80;

            OutCodes *somCodesMatrix = read_matrix_file("sspy_som_trained_1_80_80.cod", 0, dim);

            graphics::UMatChart<double, char> umatChart(733, 733);
            umatChart.setWindowTitle("U-Matrix");
            graphics::ChartThread<graphics::UMatCell<double>> umchartThread(&umatChart);
            drawUMat(somCodesMatrix, umatChart, xdim, ydim, dim);

            // init reader
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            SspyRowParser rowParser;
            file::stream::CsvFileStreamReader<SspyData, double> dataReader(&reader, &rowParser);
            kohonen::winner::EuclideanWinnerSearch<double, double> winnerSearcher;
            size_t winnerSize = winnerSearcher.getWinnerSize();

            utils::hash::CharHash cHash;
            kohonen::labeling::SomLabeling<char> somLabeling(xdim, ydim, &cHash);

            models::DataSample<double> samples[dim];
            SspyData rowData;
            size_t indexRow = 0;
            while (dataReader.readNext(rowData, samples)) {
                kohonen::winner::WinnerInfo<double> winners[winnerSize];
                bool ok = winnerSearcher.search(somCodesMatrix, samples, winners);
                if (ok) {
//                    std::cout << " " << winners[0].index;
                    somLabeling.addWinner(winners[0].index, rowData.msg_type);
                }
                if (indexRow!=0 && indexRow%40000==0) {
                    std::cout<<indexRow<<std::endl;
                    drawUMatWithLabels(somCodesMatrix, somLabeling, umatChart, xdim, ydim, dim);
                    // clean drawed data for labels
//                    somLabeling.cleanWinnerLabels();
                }
                indexRow++;
            }



            umatChart.saveImage("sspy_u_matrix_labeled_80_80_after_trained.png");

            graphics::UMatChart<double, char> umatChart2(8000, 8000);
            umatChart2.setWindowTitle("U-Matrix");
            graphics::ChartThread<graphics::UMatCell<double>> umchartThread2(&umatChart2);
            drawUMatWithLabels(somCodesMatrix, somLabeling, umatChart2, xdim, ydim, dim);
            umatChart2.saveImage("sspy_u_matrix_labeled_big_80_80_after_trained.png");

//            graphics::SammonMapChart<double> sammonChart(xdim, 733, 733);
//            sammonChart.setWindowTitle("Sammon map for trained codes");
//            graphics::ChartThread<bool> sammonChartThread(&sammonChart);
//            kohonen::SammonMap<double>* sammonMap = buildAndShowSammonMap(somCodesMatrix, sammonChart, 100);
//
//            graphics::SammonMapChart<double> sammonChartImg(xdim, 8000, 8000);
//            sammonChartImg.setWindowTitle("Sammon map for initialized codes img");
//            graphics::ChartThread<bool> sammonChartThread2(&sammonChartImg);
//            sammonChartImg.addSammonMapPoints(sammonMap->getMapPoints());
//            sammonChartImg.saveImage("sspy_sammon_map_80_80_after_trained_img2.png");

            delete somCodesMatrix;
//            delete sammonMap;

            double summaryTime = get_time() - start;
            printf("Visualization lableing trained codes time: %f\n", summaryTime / 60.0);
        }

        void test_copy_part_of_sspy_file() {
            double start = get_time();

            save_part_of_file(BIG_DATA_FILE_PATH, "content_data_100.cod", 100);
            save_part_of_file(BIG_DATA_FILE_PATH, "content_data_1000.cod", 1000);
            save_part_of_file(BIG_DATA_FILE_PATH, "content_data_10000.cod", 10000);
            save_part_of_file(BIG_DATA_FILE_PATH, "content_data_100000.cod", 100000);
            save_part_of_file(BIG_DATA_FILE_PATH, "content_data_400000.cod", 400000);
            save_part_of_file(BIG_DATA_FILE_PATH, "content_data_1000000.cod", 1000000);

            double summaryTime = get_time() - start;
            printf("Copy part of file time: %f\n", summaryTime / 60.0);
        }

        void sspy_data_read_test() {
            suite("Sspy_test");
//            mytest(read_sspy_data_file_by_line);
//            mytest(csv_parse_sspy_data_file);
//            mytest(collect_summary_sspy_data_file);
//            mytest(initialization_codes_sspy);
//            mytest(visible_initialized_codes_sspy);
//            mytest(trained_codes_sspy);
//            mytest(trained_codes_sspy2);
            mytest(visible_trained_codes_sspy);

//            mytest(copy_part_of_sspy_file);

//            mytest(visible_labling_trained_codes_sspy);
        }
    }
}



