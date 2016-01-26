#ifndef SRC_KOHONEN_ARRAYSTREAMREADER_H
#define SRC_KOHONEN_ARRAYSTREAMREADER_H

#include <stdio.h>

#include "file/CsvFileReader.h"
#include "utils/SMatrix.h"

namespace kohonen {

    template<typename T>
    class ArrayStreamReader {
    public:

        ArrayStreamReader(file::CsvFileReader<char> *_csvReader,
                          void (*_readInitialization)(
                                  file::CsvFileReader<char> *), size_t _colSize,
                          bool _useBuffer) :
                csvReader(_csvReader), readInitialization(_readInitialization),
                useBuffer(_useBuffer), rowIndex(0), colSize(_colSize),
                bufferMatrix(nullptr), typeSizeof(sizeof(T)) {
            if (useBuffer) {
                bufferMatrix = new utils::SMatrix<T>(0, colSize);
            }
            readInitialization(csvReader);
        }

        ~ArrayStreamReader() {
            if (bufferMatrix) {
                delete bufferMatrix;
            }
        }

        bool readNext(T *a) {
            if (useBuffer && rowIndex<bufferMatrix->getRowSize()) {
                T* row = bufferMatrix->getRow(rowIndex);
                memcpy(a, row, colSize * typeSizeof);
                rowIndex++;
                return true;
            }

            if (!csvReader->hasNext()) {
                return false;
            }

            for (size_t i = 0; i < colSize; ++i) {
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

        size_t getColSize() {
            return colSize;
        }

    private:
        file::CsvFileReader<char> *csvReader;

        // указатель на функцию инициализации чтения
        void (*readInitialization)(file::CsvFileReader<char> *);

        // variable for buffering
        utils::SMatrix<T> *bufferMatrix;
        size_t rowIndex;
        size_t colSize;
        bool useBuffer;
        size_t typeSizeof;
    };
}


#endif
