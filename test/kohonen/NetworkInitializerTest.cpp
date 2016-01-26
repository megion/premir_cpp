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
            kohonen::ArrayStreamReader<float> dataReader(&csvReader,
                                                         readInitializer);

            kohonen::
            NetworkInitializer<kohonen::ArrayStreamReader<float>, float, double>
                    initializer(&dataReader);

//            std::cout.precision(std::numeric_limits<double>::digits10);

            utils::SMatrix<double> *resultsMatrix =
                    initializer.lineInitialization(16, 12, 5);

            resultsMatrix->print();

            delete resultsMatrix;
        }

        void network_initializer_test() {
            suite("NetworkInitializer_test");
            test(line_initialization);
        }
    }
}



