#include "FileReaderTest.h"

namespace test {
    namespace file_reader {
        void test_read_file() {
//            file::LineFileReader<char> fr(
//                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");
            // 17/11/2014 00:27:13.661
            file::LineFileReader<char> fr(
                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");

            file::LineFileReader<char>::LineBuffer buf;
            while (fr.readNextLine(&buf, true) && fr.getLineNumber()<100) {
                std::cout << buf.buffer << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
                std::free(buf.buffer);
            }
        }

        void file_reader_test() {
            suite("FileReader_test");
//            test(read_file);
        }
    }
}



