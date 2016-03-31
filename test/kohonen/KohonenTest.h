#ifndef TEST_KOHONEN_KOHONENTEST_H
#define TEST_KOHONEN_KOHONENTEST_H

#include <iostream>
#include <cmath>

#include "test.h"
#include "kohonen/NetworkInitializer.h"
#include "file/CsvFileReader.h"
#include "file/CsvFileRowParser.h"
#include "file/stream/CsvFileStreamReader.h"
#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "utils/hash/StringHash.h"
#include "kohonen/RandomGenerator.h"
#include "kohonen/winner/EuclideanWinnerSearch.h"
#include "kohonen/alphafunc/InverseAlphaFunction.h"
#include "kohonen/alphafunc/LinearAlphaFunction.h"

#include "kohonen/neighadap/BubbleNeighborAdaptation.h"
#include "kohonen/neighadap/GaussianNeighborAdaptation.h"

#include "kohonen/mapdist/HexaMapDistance.h"
#include "kohonen/mapdist/RectMapDistance.h"

#include "kohonen/SomTrainer.h"
#include "kohonen/SammonMap.h"
#include "kohonen/labeling/SomLabeling.h"

#include "kohonen/umat/UMat.h"
#include "kohonen/umat/RectUMat.h"
#include "kohonen/umat/HexaUMat.h"

#include "graphics/ChartThread.h"
#include "graphics/SammonMapChart.h"
#include "graphics/PointChart.h"
#include "graphics/UMatChart.h"

namespace test {
    namespace kohonen_test {

        struct DemoInRow {
            bool value1;
            char label;
        };

        struct DemoStringKey {
            char label[20];
            size_t length;
            void setLabel(const char* val) {
                size_t len = std::strlen(val);
                std::memcpy(label, val, len);
                label[len]='\0';
                length = len;
            }

            // compare function for hash map
            bool operator==(const DemoStringKey &other) const {
                if(length!=other.length) {
                    return false;
                }
                return std::strncmp(label, other.label, length)==0;
            }
            bool operator!=(const DemoStringKey &other) const {
                return !((*this) == other);
            }


        };

//        std::ostream &operator<<(std::ostream &os, const DemoStringKey &val) {
//            os << "length {" << val.length << "}, label {" << val.label << "}";
//            return os;
//        }

        class DemoStringKeyHash : public utils::hash::HashEngine<DemoStringKey> {
        public:
            DemoStringKeyHash() : stringHash(utils::hash::StringHash<const char*>()) {
            }
            size_t hashCode(const DemoStringKey &value) const {
                return stringHash.hashCode(value.label);
            }
        private:
            utils::hash::StringHash<const char*> stringHash;
        };

        struct DemoInRowFts {
            DemoStringKey ftsLabel;
        };

        class KohonenDemoCsvFileRowParser : public file::CsvFileRowParser<DemoInRow> {

            bool parseRow(DemoInRow &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
                size_t colSize = 5;
                for (size_t i = 0; i < colSize; ++i) {
                    readNextDataSample(samples[i], csvReader);
                }

                return true;
            }

            void initReadFile(file::CsvFileReader *csvReader) {
                // skip first line
                csvReader->toEndLine();
            }

        private:

            bool isSampleSkipped(char *buffer, size_t bytesRead) {
                if (bytesRead == 0 || buffer[0] == 'x' || buffer[0] == 'X') {
                    return true;
                } else {
                    return false;
                }
            }

            size_t readNextDataSample(models::DataSample &sample, file::CsvFileReader *csvReader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (bytesRead == 0 || isSampleSkipped(buffer, bytesRead)) {
                    sample.skipped = true;
                } else {
                    sample.skipped = false;
                    sample.value = std::atof(buffer);
                }
                return bytesRead;
            }

        };

        class KohonenDemo2CsvFileRowParser : public file::CsvFileRowParser<DemoInRow> {

            bool parseRow(DemoInRow &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
                size_t colSize = 20;
                for (size_t i = 0; i < colSize; ++i) {
                    readNextDataSample(samples[i], csvReader);
                }

                return true;
            }

            void initReadFile(file::CsvFileReader *csvReader) {
                // skip first line
                csvReader->toEndLine();
                csvReader->toEndLine();
            }

        private:

            size_t readNextDataSample(models::DataSample &sample, file::CsvFileReader *csvReader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (bytesRead == 0 || buffer[0] == 'x' || buffer[0] == 'X') {
                    sample.skipped = true;
                } else {
                    sample.skipped = false;
                    sample.value = std::atof(buffer);
                }
                return bytesRead;
            }

        };

        class DemoFtsCsvFileRowParser : public file::CsvFileRowParser<DemoInRowFts> {

            bool parseRow(DemoInRowFts &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
                size_t colSize = 5;
                for (size_t i = 0; i < colSize; ++i) {
                    readNextDataSample(samples[i], csvReader);
                }

                // get label
                row.ftsLabel.label[0] = '\0';
                size_t bytesRead = csvReader->read(row.ftsLabel.label, 20);
                if (bytesRead == 0) {
                    row.ftsLabel.label[0] = '\0';
                }
                row.ftsLabel.length = std::strlen(row.ftsLabel.label);

                return true;
            }

            void initReadFile(file::CsvFileReader *csvReader) {
                // skip first line
                csvReader->toEndLine();
            }

        private:

            size_t readNextDataSample(models::DataSample &sample, file::CsvFileReader *csvReader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (bytesRead == 0 || buffer[0] == 'x' || buffer[0] == 'X') {
                    sample.skipped = true;
                } else {
                    sample.skipped = false;
                    sample.value = std::atof(buffer);
                }
                return bytesRead;
            }

        };

        void kohonen_test();
    }
}

#endif

