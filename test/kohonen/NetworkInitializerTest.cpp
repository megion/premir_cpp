#include "NetworkInitializerTest.h"

namespace test {
    namespace kohonen_initializer {

        void readInitializer(file::CsvFileReader<char> *csvReader) {
            // skip first line
            csvReader->toEndLine();
        }

        utils::SMatrix<double> *read_codes_file(const char *filename,
                                                int skipLines) {
            file::CsvFileReader<char> reader(filename, ' ');
            utils::SMatrix<double> *somCodesMatrix =
                    new utils::SMatrix<double>(0, 5);
            // skip to lines
            for (int i = 0; i < skipLines; ++i) {
                reader.toEndLine();
            }

            while (reader.hasNext()) {
                double row[5];
                reader.read(row[0]);
                reader.read(row[1]);
                reader.read(row[2]);
                reader.read(row[3]);
                reader.read(row[4]);
                reader.toEndLine();
                if (!reader.isEmptyRead()) {
                    somCodesMatrix->pushRow(row);
                }
            }

            return somCodesMatrix;
        }

        utils::SMatrix<double> *read_some_initilized_codes() {
            return read_codes_file(
                    "../test/datafiles/kohonen/som_initialized.cod", 2);
        }

        void test_line_initialization() {
            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader(
                    "../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<double>
                    dataReader(&csvReader, readInitializer, 5, true);

            kohonen::
            NetworkInitializer<double, double> initializer(&dataReader);
            kohonen::RandomGenerator *randomEngine
                    = initializer.getRandomGenerator();
            // TODO: в тестах для генерации кодов нейронов таких же как и в
            // проверочном файле 'som_initialized.cod', необходимо задавать
            // начальное рандомное число setNextValue(1) т.к. файл
            // 'som_initialized.cod' был сгенерирован при начальном значении = 1
            randomEngine->setNextValue(1);
            //randomEngine->initGenerator();

//            std::cout.precision(std::numeric_limits<double>::digits10);

            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            utils::SMatrix<double> *resultsMatrix =
                    initializer.lineInitialization(xdim, ydim, dim);

            utils::SMatrix<double> *somCodesMatrix = read_some_initilized_codes();
//            somCodesMatrix->print();
//            resultsMatrix->print();

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*resultsMatrix, 0.001));

            // test NAN
//            double a = 5.6;
//            assert(!std::isnan(a));
//            a = NAN;
//            assert(std::isnan(a));

//            std::cout << "test""ff"" ""C"" AB" << std::endl;

//            somCodesMatrix->print();


            delete somCodesMatrix;
            delete resultsMatrix;
        }

        void test_eucw_bubble_hexa_16_12_som_training() {

            // инициализация потока чтения файла с данными
            file::CsvFileReader<char> csvReader(
                    "../test/datafiles/kohonen/ex.dat", ' ');
            file::stream::CsvFileArrayStreamReader<double>
                    dataReader(&csvReader, readInitializer, 5, true);

            size_t xdim = 16;
            size_t ydim = 12;
            size_t dim = 5;

            utils::SMatrix<double> *somCodesMatrix =
                    read_some_initilized_codes();

            kohonen::winner::EuclideanWinnerSearch<double, double>
                    winnerSearcher;
            kohonen::alphafunc::LinearAlphaFunction<double> alphaFunc;
            kohonen::mapdist::HexaMapDistance<double> mapDist;

            kohonen::neighadap::BubbleNeighborAdaptation<double, double>
                    neiAdap(&mapDist, xdim, ydim);
            kohonen::SomTrainer<double, double> trainer(&dataReader,
                                                        &alphaFunc,
                                                        &winnerSearcher,
                                                        &neiAdap,
                                                        0.002, 3, xdim, ydim);

            trainer.training(somCodesMatrix, 10000, dim);

            utils::SMatrix<double> *expectedCodesMatrix =
                    read_codes_file("../test/datafiles/kohonen/som_trained_"
                                            "eucw_bubble_hexa_16_12.cod", 1);

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*expectedCodesMatrix,
                                                   0.001));

//            expectedCodesMatrix->print();
//            somCodesMatrix->print();

            delete expectedCodesMatrix;
            delete somCodesMatrix;
        }

        void network_initializer_test() {
            suite("NetworkInitializer_test");
            mytest(line_initialization);
            mytest(eucw_bubble_hexa_16_12_som_training);
        }
    }
}



