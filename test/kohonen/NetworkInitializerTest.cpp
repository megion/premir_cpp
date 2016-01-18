#include "NetworkInitializerTest.h"

namespace test {
    namespace kohonen_initializer {

        void test_line_initialization() {
            file::CsvFileReader<char> reader(
                    "../test/datafiles/kohonen/ex.dat", ' ');
            utils::SMatrix<float> dataMatrix(0, 5);
            reader.toEndLine();
            while (reader.hasNext()) {
                float row[5];
                reader.read(row[0]);
                reader.read(row[1]);
                reader.read(row[2]);
                reader.read(row[3]);
                reader.read(row[4]);
//                dataReader.toEndLine();


                if (!reader.isEmptyRead()) {
                    dataMatrix.pushRow(row);
//                    std::cout << "lineNumber: " << reader.getLineNumber() <<
//                    " counter: " << row.counter << " msg_type: " <<
//                    row.msg_type << " stream_type: " << row.stream_type <<
//                    " date: " << row.date << " time: " << row.time <<
//                    " interval: " << row.interval << std::endl;
                }
            }


//            std::cout << "std::numeric_limits<double>::digits: " <<
//            std::numeric_limits<unsigned long long int>::digits << std::endl;
//            std::cout << "std::numeric_limits<double>::digits10: " <<
//            std::numeric_limits<double>::digits10 << std::endl;
//            std::cout << "std::numeric_limits<double>::max_digits10: " <<
//            std::numeric_limits<double>::max_digits10 << std::endl;

            kohonen::NetworkInitializer<float, double> initializer;

            std::cout.precision(std::numeric_limits<double>::digits10);

            initializer.lineInitialization(dataMatrix);

//            dataMatrix.print();
        }

        void network_initializer_test() {
            suite("NetworkInitializer_test");
            test(line_initialization);
        }
    }
}



