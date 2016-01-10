#include "FileReaderTest.h"

namespace test {
    namespace file_reader {
        void test_read_file() {
            file::FileReader<char> fr(
                    "/home/ilya/workspaces/luna/premir_cpp/LICENSE");
            file::FileReader<char>::LineBuffer buf;
            while(fr.readNextLine(&buf, true)) {
                std::cout << buf.buffer << std::endl;
                std::free(buf.buffer);
            }
        }

        void file_reader_test() {
            suite("FileReader_test");
            test(read_file);
        }
    }
}



