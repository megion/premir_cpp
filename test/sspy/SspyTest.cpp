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

        void test_read_sspy_data_file_by_line() {
//            file::LineFileReader<char> fr(
//                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");
            // 17/11/2014 00:27:13.661
            // /run/media/ilya/Elements/SmartSpy/\[Content\]
            file::LineFileReader<char> lineReader(BIG_DATA_FILE_PATH);

            file::LineFileReader<char>::LineBuffer buf;
            while (lineReader.readNextLine(&buf, true) &&
                   lineReader.getLineNumber() <= 3) {
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

        void buildAndShowSammonMap(OutCodes *somMatrix,
                                   graphics::SammonMapChart<double> &sammonChart) {
            kohonen::SammonMap<double> sammonMap(somMatrix->getRowSize());
            kohonen::RandomGenerator *randomEngine = sammonMap.getRandomGenerator();
            // для теста псевдоинициализация
            randomEngine->setNextValue(1);
            sammonMap.initializeMap(somMatrix);

            for (size_t i = 0; i < 200; ++i) {
                sammonChart.removeDataSafely();
                sammonMap.doOneIteration();
                sammonChart.addSammonMapPoints(sammonMap.getMapPoints());
                sammonChart.drawOnWindow();
            }
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
            printf("Summary time: %f\n", summaryTime/60.0);
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
            printf("Initialization time: %f\n", summaryTime/60.0);
        }

        void test_visible_initialized_codes_sspy() {
            double start = get_time();
            size_t dim = 17;
            size_t xdim = 80;
//            size_t ydim = 80;

            OutCodes *somCodesMatrix = read_matrix_file("sspy_som_initialized_5_80_80.cod", dim, 0);
            graphics::SammonMapChart<double> sammonChart2(xdim, 1200, 700);
            sammonChart2.setWindowTitle("Sammon map for initialized codes");
            graphics::ChartThread<bool> sammonChartThread2(&sammonChart2);
            buildAndShowSammonMap(somCodesMatrix, sammonChart2);

            delete somCodesMatrix;

            double summaryTime = get_time() - start;
            printf("Initialization time: %f\n", summaryTime/60.0);
        }

        void sspy_data_read_test() {
            suite("Sspy_test");
//            mytest(read_sspy_data_file_by_line);
//            mytest(csv_parse_sspy_data_file);
//            mytest(collect_summary_sspy_data_file);
//            mytest(initialization_codes_sspy);
            mytest(visible_initialized_codes_sspy);
        }
    }
}



