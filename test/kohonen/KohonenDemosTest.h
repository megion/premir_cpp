#ifndef TEST_KOHONEN_KOHONENDEMOTEST_H
#define TEST_KOHONEN_KOHONENDEMOTEST_H

#include <iostream>
#include <cmath>

#include "test.h"

#include "file/CsvFileReader.h"
#include "file/CsvFileWriter.h"
#include "file/CsvFileRowParser.h"
#include "file/stream/CsvFileStreamReader.h"
#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "utils/hash/CharHash.h"
#include "utils/console_colors.h"

#include "kohonen/NetworkInitializer.h"
#include "kohonen/RandomGenerator.h"
#include "kohonen/winner/WinnerDistance.h"
#include "kohonen/winner/DefaultWinnerDistance.h"
#include "kohonen/winner/EuclideanWinnerSearch.h"
#include "kohonen/alphafunc/InverseAlphaFunction.h"
#include "kohonen/alphafunc/LinearAlphaFunction.h"

#include "kohonen/neighadap/BubbleNeighborAdaptation.h"
#include "kohonen/neighadap/GaussianNeighborAdaptation.h"

#include "kohonen/mapdist/HexaMapDistance.h"
#include "kohonen/mapdist/RectMapDistance.h"

#include "kohonen/SomTrainer.h"
#include "kohonen/SomKeeper.h"
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
    namespace kohonen_demos_test {

        struct DemoInRow {
            char label;
        };

        class SpeechLabelColorMapper : public graphics::LabelColorMapper<char> {
            graphics::Color labelToColor(const char& label) const {
                graphics::Color c;
                int l = (int)label;
                graphics::calculateWavelengthColor(l, 20, 100, c.r, c.g, c.b);
                return c;
            }

        };

        class KohonenDemo2CsvFileRowParser : public file::CsvFileRowParser<DemoInRow> {

            bool parseRow(DemoInRow &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
                size_t colSize = 20;
                for (size_t i = 0; i < colSize; ++i) {
                    readNextDataSample(samples[i], csvReader);
                }
                // get label
                char buffer[2];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 2);
                if (bytesRead == 0) {
                    row.label = '\0';
                } else {
                    row.label = buffer[0];
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

        class HouseVotesCsvFileRowParser : public file::CsvFileRowParser<DemoInRow> {

            bool parseRow(DemoInRow &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
                // get label
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (bytesRead == 0) {
                    row.label = '\0';
                } else {
                    row.label = buffer[0];
                }

                size_t colSize = 16;
                for (size_t i = 0; i < colSize; ++i) {
                    readNextDataSample(samples[i], csvReader);
                }

                return true;
            }

            void initReadFile(file::CsvFileReader *csvReader) {
            }

        private:

            size_t readNextDataSample(models::DataSample &sample, file::CsvFileReader *csvReader) {
                char buffer[64];
                *buffer = '\0';
                size_t bytesRead = csvReader->read(buffer, 64);
                if (bytesRead == 0) {
                    sample.skipped = true;
                } else if (buffer[0] == '?') {
                    sample.skipped = false;
                    sample.value = 1;
                } else if (buffer[0] == 'n') {
                    sample.skipped = false;
                    sample.value = 0;
                } else if (buffer[0] == 'y') {
                    sample.skipped = false;
                    sample.value = 2;
                } else {
                    sample.skipped = true;
                }
                return bytesRead;
            }

        };

        class HouseVotesWinnerDistance : public kohonen::winner::WinnerDistance {
        public:
            HouseVotesWinnerDistance() : kohonen::winner::WinnerDistance() {
            }

            bool distance(double *somNeuronCodes, models::DataSample *samples, size_t dim, double &difference,
                          const double &maxDifference) const {
                size_t masked = 0;
                difference = 0;
                /* Compute the distance between codebook and input entry */
                for (size_t i = 0; i < dim; ++i) {
                    if (samples[i].skipped) {
                        masked++;
                        /* ignore vector components that skipped */
                        continue;
                    }

                    double diff = somNeuronCodes[i] - samples[i].value;
                    difference += diff * diff;
                    if (difference > maxDifference) {
                        break;
                    }
                }

                if (masked == dim) {
                    /* TODO: can't calculate winner, empty data vector */
					LOG(ERR, "can't calculate winner, empty data vector");
                    return false;
                }

                return true;
            }

        };

        void kohonen_demos_test();
    }
}

#endif

