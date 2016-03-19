#ifndef TEST_SSPY_SSPYTEST_H
#define TEST_SSPY_SSPYTSET_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "test.h"

#include "SspyData.h"
#include "file/LineFileReader.h"
#include "file/CsvFileReader.h"


namespace test {
    namespace sspy {

        // "/home/ilya/share/Documents/ml/SmartSpy/[Content]"
        const static char* BIG_DATA_FILE_PATH = "/run/media/ilya/Elements/SmartSpy/[Content]";
        void sspy_data_read_test();
    }
}

#endif

