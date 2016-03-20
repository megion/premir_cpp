#include "SspyTest.h"

namespace test {
    namespace sspy {

        void test_read_sspy_data_file_by_line() {
//            file::LineFileReader<char> fr(
//                    "/home/ilya/share/Documents/ml/SmartSpy/[Content]");
            // 17/11/2014 00:27:13.661
            // /run/media/ilya/Elements/SmartSpy/\[Content\]
            file::LineFileReader<char> lineReader(BIG_DATA_FILE_PATH);

            file::LineFileReader<char>::LineBuffer buf;
            while (lineReader.readNextLine(&buf, true) &&
                   lineReader.getLineNumber() <= 3) {
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
            // sudo mount -t vboxsf izadorozhny ~/share
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
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

        void test_collect_summary_sspy_data_file() {
            size_t dim = 33;
            file::CsvFileReader reader(BIG_DATA_FILE_PATH, ' ');
            SspyRowParser rowParser;
            file::stream::CsvFileStreamReader<SspyData, double> dataReader(&reader, &rowParser);
            file::CsvFileSummary<SspyData, double> summary(dim);
            summary.collectSummary(0, &reader, &rowParser); // 0 - значит без ограничений
//            summary.getSummary()->print();

            // TODO: summary save
            summary.writeSummary("sspy_data_summary_1.cod");
        }

        void sspy_data_read_test() {
            suite("Sspy_test");
//            mytest(read_sspy_data_file_by_line);
//            mytest(csv_parse_sspy_data_file);
            mytest(collect_summary_sspy_data_file);
        }
    }
}



