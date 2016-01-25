

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

namespace file {

    template<typename Ch>
    class RowReader {
    public:
        RowReader(const char *filename, const Ch sep) {
        }

        ~RowReader() {
        }

    private:

    };
}


#endif
