#include "NetworkInitializerTest.h"

namespace test {
    namespace kohonen_initializer {

        void readInitializer(file::CsvFileReader<char> *csvReader) {
            // skip first line
            csvReader->toEndLine();
        }

        void test_line_initialization() {
            file::CsvFileReader<char> csvReader(
                    "../test/datafiles/kohonen/ex.dat", ' ');
            kohonen::ArrayStreamReader<double> dataReader(&csvReader,
                                                          readInitializer, 5,
                                                          true);
            kohonen::RandomGenerator randomEngine;
//            randomEngine.setNextValue(1);

            kohonen::
            NetworkInitializer<kohonen::ArrayStreamReader<double>, double, double>
                    initializer(&dataReader, randomEngine);

//            std::cout.precision(std::numeric_limits<double>::digits10);

            utils::SMatrix<double> *resultsMatrix =
                    initializer.lineInitialization(16, 12);

            resultsMatrix->print();

            delete resultsMatrix;
        }

        void network_initializer_test() {
            suite("NetworkInitializer_test");
            test(line_initialization);
        }
    }
}



