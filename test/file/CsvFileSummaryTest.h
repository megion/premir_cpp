#ifndef TEST_FILE_CSVFILESUMMARYTEST_H
#define TEST_FILE_CSVFILESUMMARYTEST_H

#include <iostream>

#include "test.h"
#include "file/CsvFileSummary.h"
#include "file/CsvFileReader.h"

namespace test {
    namespace file_csvfilesummary {

        struct MyInRow {
            bool value1;
            char value2[10];
        };

        class MyCsvFileRowParser : public file::CsvFileRowParser<MyInRow> {

            bool parseRow(MyInRow &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
                size_t colSize = 20;
                for (size_t i = 0; i < colSize; ++i) {
                    readNextDataSample(samples[i], csvReader);
                }

                return true;
            }

            void initReadFile(file::CsvFileReader *csvReader) {
                // skip 2 lines
                csvReader->toEndLine();
                csvReader->toEndLine();
            }

        private:

            size_t readNextDataSample(models::DataSample &sample, file::CsvFileReader *csvReader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (buffer[0] == 'x' || buffer[0] == 'X') {
                    sample.skipped = true;
                } else {
                    sample.skipped = false;
                    sample.value = std::atof(buffer);
                }
                return bytesRead;
            }

        };

        void file_csvFileSummary_test();
    }
}

#endif

