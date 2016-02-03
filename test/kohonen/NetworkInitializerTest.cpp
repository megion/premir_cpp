#include "NetworkInitializerTest.h"

namespace test {
    namespace kohonen_initializer {

        void readInitializer(file::CsvFileReader<char> *csvReader) {
            // skip first line
            csvReader->toEndLine();
        }

        utils::SMatrix<double> *read_some_initilized_codes() {
            file::CsvFileReader<char> reader(
                    "../test/datafiles/kohonen/som_initialized.cod", ' ');
            utils::SMatrix<double> *somCodesMatrix =
                    new utils::SMatrix<double>(0, 5);
            // skip to lines
            reader.toEndLine();
            reader.toEndLine();

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

            utils::SMatrix<double> *somCodesMatrix =
                    read_some_initilized_codes();

            // данные матрицы должны быть практически идентичными
            assert(somCodesMatrix->equalsWithError(*resultsMatrix, 0.001));

            kohonen::winner::EuclideanWinnerSearch<double, double> winnerSearcher;
            kohonen::alphafunc::BubbleNeighborAdaptation<double> alphaFunc;
            kohonen::SomTrainer<double, double> trainer(&dataReader,
                                                        &alphaFunc,
                                                        &winnerSearcher,
                                                        0.002, 3, xdim, ydim);

            trainer.training(somCodesMatrix, 10, dim);

            // test NAN
//            double a = 5.6;
//            assert(!std::isnan(a));
//            a = NAN;
//            assert(std::isnan(a));

//            std::cout << "test""ff"" ""C"" AB" << std::endl;


            delete somCodesMatrix;
            delete resultsMatrix;
        }

        void network_initializer_test() {
            suite("NetworkInitializer_test");
            mytest(line_initialization);
        }
    }
}



