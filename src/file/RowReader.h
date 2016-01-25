#ifndef SRC_FILE_CSVFILEREADER_H
#define SRC_FILE_CSVFILEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <limits>

#include "utils/console_colors.h"
#include "file/CsvFileReader.h"

namespace file {

    template<typename Ch, typename RowElement, typename RowParser>
    class RowReader {
    public:
        RowReader(const CsvFileReader<Ch>* _csvReader) :
                csvReader(_csvReader), parser(csvReader) {
        }

        ~RowReader() {
        }

        bool readNext(RowElement *a, size_t sizeArray) {
            if (!csvReader->hasNext()) {
                return false;
            }
            parser.parseRow();

        }

    private:
        CsvFileReader<Ch>* csvReader;
        RowParser parser;

    };
}


#endif
