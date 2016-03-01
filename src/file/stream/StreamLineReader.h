#ifndef SRC_FILE_STREAM_STREAMLINEREADER_H
#define SRC_FILE_STREAM_STREAMLINEREADER_H

#include <cstdio>

namespace file {
    namespace stream {

        template<typename T>
        class StreamLineReader {
        public:

            /**
             * Чтение следубщей строки
             */
            virtual bool readNextLine(T& row) = 0;

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
