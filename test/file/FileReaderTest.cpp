#include "FileReaderTest.h"

namespace test {
    namespace file_reader {
        void test_read_file() {
            file::LineFileReader fr("file.txt");

            file::LineFileReader::LineBuffer buf;
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



