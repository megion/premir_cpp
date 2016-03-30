#ifndef SRC_FILE_STREAM_STREAMREADER_H
#define SRC_FILE_STREAM_STREAMREADER_H

#include <cstdio>

#include "models/models.h"

namespace file {
    namespace stream {

        template<typename Row>
        class StreamReader {
        public:

            /**
             * Чтение данных
             */
            virtual bool readNext(Row& row, models::DataSample* samples) = 0;

            /**
             * Начать читать поток заново. В случае файла
             * устанавливается дескриптор в начало или если используется
             * буфер то буфер начинает читаться с начала.
             */
            virtual void rewindReader() = 0;

        };
    }
}


#endif
