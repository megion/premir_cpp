#ifndef SRC_FILE_STREAM_CSVFILEARRAYSTREAMREADER_H
#define SRC_FILE_STREAM_CSVFILEARRAYSTREAMREADER_H

#include <stdio.h>

#include "file/CsvFileReader.h"
#include "utils/SMatrix.h"
#include "StreamReader.h"

namespace file {

    namespace stream {

        template<typename T>
        class CsvFileArrayStreamReader : public StreamReader<T> {
        public:

            CsvFileArrayStreamReader(file::CsvFileReader<char> *_csvReader,
                                     void (*_readInitialization)
                                             (file::CsvFileReader<char> *),
                                     size_t _maxColSize, bool _useBuffer) :
                    csvReader(_csvReader),
                    readInitialization(_readInitialization),
                    useBuffer(_useBuffer),
                    rowIndex(0),
                    maxColSize(_maxColSize),
                    bufferMatrix(nullptr),
                    typeSizeof(sizeof(T)) {
                if (useBuffer) {
                    bufferMatrix = new utils::SMatrix<T>(0, maxColSize);
                }
                readInitialization(csvReader);
            }

            ~CsvFileArrayStreamReader() {
                if (bufferMatrix) {
                    delete bufferMatrix;
                    bufferMatrix = nullptr;
                }
            }

            bool readNext(T &a) {
                return true;
            }

            bool readNext(T *a, size_t arraySize) {
                if (useBuffer && rowIndex < bufferMatrix->getRowSize()) {
                    T *row = bufferMatrix->getRow(rowIndex);
                    memcpy(a, row, arraySize * typeSizeof);
                    rowIndex++;
                    return true;
                }

                if (!csvReader->hasNext()) {
                    return false;
                }

                for (size_t i = 0; i < arraySize; ++i) {
                    csvReader->read(a[i]);
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

            // variable for buffering
            utils::SMatrix<T> *bufferMatrix;
            size_t rowIndex;
            size_t maxColSize;
            bool useBuffer;
            size_t typeSizeof;
        };
    }

}


#endif
