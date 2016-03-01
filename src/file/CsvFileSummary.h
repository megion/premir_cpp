#ifndef SRC_FILE_CSVFILESUMMARY_H
#define SRC_FILE_CSVFILESUMMARY_H

#include <cstdio>
#include <cmath>

#include "file/CsvFileReader.h"
#include "utils/CArrayList.h"

namespace file {

    namespace stream {


        template<typename T>
        class CsvFileSummary {
        public:

            /**
             * Хранит некоторую статистику для одной колонки. Статистика собирается только по допустимым значениям.
             */
            struct ColSummary {
                T min; // минимальное допустимое значение
                T max; // максимальное (допустимое) значение в колонке
                double sum; // сумма допустимых значений в колонке
                size_t count; // число допустимых значенией в колонке (не все значения являются допустимыми)
            };

            CsvFileSummary(file::CsvFileReader<char> *_csvReader, size_t colSize) :
                    csvReader(_csvReader) {
                summary = new utils::CArrayList<ColSummary>(colSize);
            }

            ~CsvFileSummary() {
                delete summary;
                summary = nullptr;
            }

            void collectSummary() {
                while (dataReader->readNext(inRow, colSize)) {
                    for (size_t i = 0; i < colSize; ++i) {
                        if (!inRow[i].skipped) {
                            colMedians[i] += inRow[i].value;
                            k2[i]++;
                        }
                    }
                }
            }

            size_t readNextDataSample(models::DataSample<float> &sample) {
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

            size_t readNextDataSample(models::DataSample<double> &sample) {
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

            bool readNext(models::DataSample <T> *a, size_t arraySize) {
                if (useBuffer && rowIndex < bufferMatrix->getRowSize()) {
                    models::DataSample <T> *row = bufferMatrix->getRow(rowIndex);
                    memcpy(a, row, arraySize * typeSizeof);
                    rowIndex++;
                    return true;
                }

                if (!csvReader->hasNext()) {
                    return false;
                }

                for (size_t i = 0; i < arraySize; ++i) {
                    readNextDataSample(a[i]);
                }

                if (csvReader->isEmptyRead()) {
                    // были считаны пустые данные, значит их использовать нельзя
                    // останавливаем чтение
                    return false;
                } else {
                    if (useBuffer) {
                        bufferMatrix->pushRow(a);
                        rowIndex++;
                    }
                    return true;
                }
            }


        private:
            file::CsvFileReader<char> *csvReader;
            utils::CArrayList<ColSummary> *summary;
        };
    }

}


#endif
