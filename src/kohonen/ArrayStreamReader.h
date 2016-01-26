#ifndef SRC_KOHONEN_ARRAYSTREAMREADER_H
#define SRC_KOHONEN_ARRAYSTREAMREADER_H

#include <stdio.h>

#include "file/CsvFileReader.h"

namespace kohonen {

    template<typename T>
    class ArrayStreamReader {
    public:
        ArrayStreamReader(file::CsvFileReader<char> *_csvReader,
                          void (*_readInitialization)(
                                  file::CsvFileReader<char> *)) :
                csvReader(_csvReader), readInitialization(_readInitialization) {
            readInitialization(csvReader);
        }

        bool readNext(T *a, size_t sizeArray) {
            if (!csvReader->hasNext()) {
                return false;
            }

            for (size_t i=0; i<sizeArray; ++i) {
                csvReader->read(a[i]);
            }

            if (csvReader->isEmptyRead()) {
                // были считаны пустые данные, значит их использовать нельзя
                // останавливаем чтение
                return false;
            } else {
                return true;
            }
        }

        void rewindReader() {
            csvReader->rewindReader();
            readInitialization(csvReader);
        }

    private:
        file::CsvFileReader<char> *csvReader;

        // указатель на функцию инициализации чтения
        void (*readInitialization)(file::CsvFileReader<char> *);
    };
}


#endif
