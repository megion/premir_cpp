#include "FileReaderTest.h"

namespace test {
    namespace file_reader {
        void test_read_file() {
//            file::FileReader<char> fr(
//                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");
            // 17/11/2014 00:27:13.661
            file::FileReader<char> fr(
                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");

            file::FileReader<char>::LineBuffer buf;
            while (fr.readNextLine(&buf, true) && fr.getLineNumber()<100) {
                std::cout << buf.buffer << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
                std::free(buf.buffer);
            }
        }

        void test_csv_read_file() {
            file::CsvFileReader<char> reader(
                    "../test/datafiles/ex1.dat", ' ');
            reader.toEndLine();
            reader.toEndLine();
            while (reader.hasNext()) {
                double val1, val2;
                reader >> val1 >> val2;
                reader.toEndLine();

                if (reader.getLineNumber()>1900) {
                    if (!reader.isEmptyRead()) {
                        std::cout << "lineNumber: " << reader.getLineNumber() <<
                        " val1: " << val1 << " val2: " << val2 << std::endl;
                    }
                }

            }
        }

        void file_reader_test() {
            suite("FileReader_test");
            test(read_file);
//            test(csv_read_file);
        }
    }
}



