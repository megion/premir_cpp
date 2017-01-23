#ifndef SRC_FILE_CSVFILESUMMARY_H
#define SRC_FILE_CSVFILESUMMARY_H

#include <cstdio>
#include <cmath>
#include <limits>

#include "file/CsvFileReader.h"
#include "file/CsvFileWriter.h"
#include "file/CsvFileRowParser.h"
#include "file/stream/CsvFileStreamReader.h"
#include "utils/CArrayList.h"
#include "models/models.h"

namespace file {

    class SummaryCsvFileRowParser : public file::CsvFileRowParser<models::ColSummary> {
        bool parseRow(models::ColSummary &row, models::DataSample *samples, file::CsvFileReader *csvReader) {
            csvReader->read(row.average);
            csvReader->read(row.scaledAverage);
            csvReader->read(row.count);
            csvReader->read(row.max);
            csvReader->read(row.min);
            csvReader->read(row.sum);
            return true;
        }

        void initReadFile(file::CsvFileReader *csvReader) {
        }

    };

    template<typename Row>
    class CsvFileSummary {
    public:

        CsvFileSummary(size_t _colSize): colSize(_colSize) {
            summary = new utils::CArrayList<models::ColSummary>(colSize, 1);
        }

        ~CsvFileSummary() {
            delete summary;
            summary = nullptr;
			colSize = 0;
        }

        /**
         * Если rowsLimit > 0 тогда статистика будет посчитана только для этого количества строк.
         */
        void collectSummary(size_t rowsLimit, file::CsvFileReader *csvReader,
                            file::CsvFileRowParser<Row> *rowParser) {
            file::stream::CsvFileStreamReader<Row> reader(csvReader, rowParser);
            initialization();

            Row row;
            models::DataSample samples[colSize];
            size_t rowIndex = 0;
            while (reader.readNext(row, samples) && (rowsLimit == 0 || (rowIndex < rowsLimit))) {
                // print process
                if(rowIndex!=0) {
                    if (rowIndex%100000==0) {
                        std::cout<<".";
                    }
                    if(rowIndex%1000000==0) {
                        std::cout<<std::endl<<rowIndex<<std::endl;
                    }
                }

                for (size_t i = 0; i < colSize; ++i) {
                    models::ColSummary &colSummary = (*summary)[i];
                    models::DataSample &sample = samples[i];
                    if (!sample.skipped) {
                        colSummary.count++;
                        colSummary.sum += sample.value;
                        if (sample.value > colSummary.max) {
                            colSummary.max = sample.value;
                        }
                        if (sample.value < colSummary.min) {
                            colSummary.min = sample.value;
                        }
                    }
                }
                rowIndex++;
            }

            // вычислим среднее
            for (size_t i = 0; i < colSize; ++i) {
                models::ColSummary &colSummary = (*summary)[i];
                if (!colSummary.isSkip()) {
                    colSummary.average = colSummary.sum / (double) colSummary.count;
                    colSummary.scaledAverage = (colSummary.average - colSummary.min) / (colSummary.max - colSummary.min);
                }
            }

        }

        utils::CArrayList<models::ColSummary> *getSummary() {
            return summary;
        }

        /**
         * val = (val - min) / (max - min)
         */
        void scaleSamples(models::DataSample *samples) {
            for (size_t i = 0; i < colSize; ++i) {
                models::ColSummary &colSummary = (*summary)[i];
                if (colSummary.isSkip()) {
                    LOG(WARN, "all data column is skip. Empty col should be removed from samples. Col: %d", i);
                }

                samples[i].value = (samples[i].value - colSummary.min) / (colSummary.max - colSummary.min);
                if (samples[i].value > 1) {
                    samples[i].value = 1;
                }
                if (samples[i].value < 0) {
                    samples[i].value = 0;
                }
            }
        }

        // TODO need redevelop
        void skipEmptyColSamples(models::DataSample *samples) {
            for (size_t i = 0; i < colSize; ++i) {
                models::ColSummary &colSummary = (*summary)[i];
                if (colSummary.isSkip()) {
                    // статистики нет по колонке.
                    samples[i].skipped = true;
                }
            }
        }

        void writeSummary(const char *filename) {
            file::CsvFileWriter writer(filename);

            for (size_t i = 0; i < colSize; ++i) {
                models::ColSummary &colSummary = (*summary)[i];
                writer.write(colSummary.average);
                writer.write(colSummary.scaledAverage);
                writer.write(colSummary.count);
                writer.write(colSummary.max);
                writer.write(colSummary.min);
                writer.write(colSummary.sum);
                writer.endLine();
            }
        }

        void readSummary(const char *filename) {
            file::CsvFileReader reader(filename, ' ');
            SummaryCsvFileRowParser summaryRowParser;
            file::stream::CsvFileStreamReader<models::ColSummary> dataReader(&reader, &summaryRowParser);

            initialization();
            models::ColSummary row;
            size_t col = 0;
            while ((col < colSize) && dataReader.readNext((*summary)[col], nullptr)) {
                col++;
            }
        }


    private:
        utils::CArrayList<models::ColSummary> *summary;
		size_t colSize;

        void initialization() {
            double min = -std::numeric_limits<double>::max();
            double max = std::numeric_limits<double>::max();
            // initialization
            for (size_t i = 0; i < colSize; ++i) {
                models::ColSummary &colSummary = (*summary)[i];
                colSummary.count = 0;
                colSummary.sum = 0;
                colSummary.average = 0;
                colSummary.scaledAverage = 0;
                colSummary.max = min; // init max by min value
                colSummary.min = max; // init mim by max value
            }
        }
    };

}


#endif
