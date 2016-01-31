#ifndef SRC_FILE_STREAM_STREAMREADER_H
#define SRC_FILE_STREAM_STREAMREADER_H

#include <cstdio>

namespace file {
    namespace stream {

        template<typename T>
        class StreamReader {
        public:

            /**
             * Чтение в массив данных
             */
            virtual bool readNext(T *a, size_t arraySize) = 0;
            /**
             * Чтение в произвольный объект
             */
            virtual bool readNext(T& a) = 0;

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
