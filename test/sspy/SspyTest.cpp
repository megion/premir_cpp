#include "SspyTest.h"

namespace test {
    namespace sspy {
        void test_read_sspy_data_file_by_line() {
//            file::LineFileReader<char> fr(
//                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");
            // 17/11/2014 00:27:13.661
            file::LineFileReader<char> lineReader(
                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");

            file::LineFileReader<char>::LineBuffer buf;
            while (lineReader.readNextLine(&buf, true) &&
                   lineReader.getLineNumber() <= 10) {
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
            file::CsvFileReader reader(
                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]", ' ');
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

        void sspy_data_read_test() {
            suite("Sspy_test");
//            mytest(read_sspy_data_file_by_line);
//            mytest(csv_parse_sspy_data_file);
        }
    }
}



