#ifndef SRC_FILE_STREAM_CSVFILEARRAYSTREAMREADER_H
#define SRC_FILE_STREAM_CSVFILEARRAYSTREAMREADER_H

#include <cstdio>
#include <cmath>

#include "file/CsvFileReader.h"
#include "utils/SMatrix.h"
#include "StreamReader.h"
#include "models/DataSample.h"

namespace file {

    namespace stream {

        template<typename T>
        class CsvFileArrayStreamReader : public StreamReader<models::DataSample<T>> {
        public:

            CsvFileArrayStreamReader(file::CsvFileReader<char> *_csvReader,
                                     void (*_readInitialization)(file::CsvFileReader<char> *),
                                     bool (*_isSampleSkipped)(char *, size_t),
                                     size_t _maxColSize, bool _useBuffer) :
                    csvReader(_csvReader),
                    readInitialization(_readInitialization),
                    isSampleSkipped(_isSampleSkipped),
                    useBuffer(_useBuffer),
                    rowIndex(0),
                    maxColSize(_maxColSize),
                    bufferMatrix(nullptr),
                    typeSizeof(sizeof(models::DataSample<T>)) {
                if (useBuffer) {
                    bufferMatrix =
                            new utils::SMatrix<models::DataSample<T>>
                                    (0, maxColSize);
                }
                readInitialization(csvReader);
            }

            ~CsvFileArrayStreamReader() {
                if (bufferMatrix) {
                    delete bufferMatrix;
                    bufferMatrix = nullptr;
                }
            }

            bool readNext(models::DataSample<T> &a) {
                return true;
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

            bool readNext(models::DataSample<T> *a, size_t arraySize) {
                if (useBuffer && rowIndex < bufferMatrix->getRowSize()) {
                    models::DataSample<T> *row = bufferMatrix->getRow(rowIndex);
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

            void rewindReader() {
                if (useBuffer) {
                    rowIndex = 0;
                } else {
                    csvReader->rewindReader();
                    readInitialization(csvReader);
                }
            }

            size_t getMaxColSize() {
                return maxColSize;
            }

        private:
            file::CsvFileReader<char> *csvReader;

            // указатель на функцию инициализации чтения
            void (*readInitialization)(file::CsvFileReader<char> *);

            // указатель на функцию проверки прочитанного значения
            bool (*isSampleSkipped)(char *, size_t);

            // variable for buffering
            utils::SMatrix<models::DataSample<T>> *bufferMatrix;
            size_t rowIndex;
            size_t maxColSize;
            bool useBuffer;
            size_t typeSizeof;
        };
    }

}


#endif
