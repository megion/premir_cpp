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

#include <sys/time.h>

#include "test.h"

#include "SspyData.h"
#include "file/LineFileReader.h"
#include "file/CsvFileReader.h"


namespace test {
    namespace sspy {

        const static char *BIG_DATA_FILE_PATH = "/home/ilya/share/Documents/ml/SmartSpy/[Content]";

//        const static char* BIG_DATA_FILE_PATH = "/run/media/ilya/Elements/SmartSpy/[Content]";
        void sspy_data_read_test();

        class MsgTypeLabelColorMapper : public graphics::LabelColorMapper<char> {
            graphics::Color labelToColor(const char &label) const {
                //
                if (label == 'L') {
                    return graphics::Color(255*255,	255*176, 255*46); // channel leave 255	176	46
                } else if (label == 'N') {
                    return graphics::Color(0, 0, 65535); // new channel
                } else if (label == 'V') {
                    return graphics::Color(65535, 0, 0); // video lost - red
                } else if (label == 'S') {
                    return graphics::Color(0, 65535, 0); // video restored - green
                } else if (label == 'K') {
                    return graphics::Color(255*120, 255*219, 255*226); // keep alive - Аквамариновый Крайола
                } else if (label == 'Z') {
                    return graphics::Color(255*229,	255*43, 255*80); // video lost keep alive Амарантовый 229	43	80
                } else if (label == 'Y') {
                    return graphics::Color(255*255, 255*140, 0); // Dark Orange 	#FF8C00 	255 140 0
                } else {
                    return graphics::Color(65535, 0, 0); // Olive 	#808000 	128 128 0
                }
            }

        };
    }
}

#endif

