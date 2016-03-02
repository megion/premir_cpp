#ifndef SRC_FILE_CSVFILEROWPARSER_H
#define SRC_FILE_CSVFILEROWPARSER_H

#include <cstdio>

#include "CsvFileReader.h"
#include "models/models.h"

namespace file {

    template<typename Row, typename Num>
    class CsvFileRowParser {
    public:

        /**
         * Разбор строки в объект row и в числовой массив samples
         */
        virtual bool parseRow(Row& row, models::DataSample<Num>* samples, CsvFileReader<char> *csvReader) = 0;

        /**
         * Инициализация чтения файла. Может быть необходима например пропустить часть строчек в начале файла.
         */
        virtual void initReadFile(CsvFileReader<char> *csvReader) = 0;

    };

}


#endif
