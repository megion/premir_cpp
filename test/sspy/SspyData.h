#ifndef TEST_SSPY_SSPYDATA_H
#define TEST_SSPY_SSPYDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "test.h"


namespace test {
    namespace sspy {

        struct SspyData {
            size_t counter;
            char msg_type;
            char stream_type;
            char date[11]; // DD/MM/YYYY
            char time[13]; // HH:MM:SS.mmm
            char interval[10]; // MM:SS.mmm
        };

        void sspy_data_read_test();
    }
}

#endif

