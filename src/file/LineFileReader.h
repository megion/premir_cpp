//
// Created by ilya on 1/8/16.
//

#ifndef SRC_FILE_FILEREADER_H
#define SRC_FILE_FILEREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/CArrayList.h"
#include "utils/console_colors.h"

namespace file {

    template<typename T>
    class LineFileReader {
    public:
        LineFileReader(const char *filename) : typeSizeof(sizeof(T)),
                                           lineNumber(0) {
            fp = fopen(filename, "r");
            if (fp == NULL) {
                std::string errMsg(std::strerror(errno));
                errMsg = errMsg + ": " + filename;
                throw std::runtime_error(errMsg);
            }
        }

        LineFileReader(const LineFileReader<T> &) = delete;
        LineFileReader(LineFileReader<T> &&) = delete;
        LineFileReader<T> &operator=(const LineFileReader<T> &) = delete;
        LineFileReader<T> &operator=(LineFileReader<T> &&) = delete;

        ~LineFileReader() {
            close();
        }

        struct LineBuffer {
            T *buffer;
            size_t size;
        };

        size_t getLineNumber() {
            return lineNumber;
        }

        bool hasNext() {
            return !isEnd;
        }

        void close() {
            if (fp) {
                if (fclose(fp) < 0) {
                    throw std::runtime_error(std::strerror(errno));
                }
                fp = nullptr;
            }
        }

        void calculateTotalLineNumber() {
            int ch;
            for (; ;) {
                ch = fgetc(fp);
                if (ch == EOF) {
                    if (feof(fp)) {
                        isEnd = true;
                    } else {
                        // error occurs
                        danger_text("error: some error occurs");
                        throw std::runtime_error(std::strerror(errno));
                    }
                    return;
                }

                if (ch == '\n') {
                    ++lineNumber;
                }
            }
        }

        bool readNextLine(LineBuffer *result,
                          bool removeNewLineSymbol = false) {
            // init buffer
            size_t bufferLen = 256;
            size_t increaseBufferValue = 64;
            size_t amount = bufferLen * typeSizeof;
            T *buffer = (T *) std::malloc(amount);
            if (buffer == NULL) {
                throw std::runtime_error(std::strerror(errno));
            }

            T *p = buffer;
            *p = '\0'; // set NUL
            T *end = buffer + bufferLen;
            while (std::fgets(p, end - p, fp) != 0) { // fgetws
                size_t len = std::strlen(p); // std::wcslen

                if (len == 0) {
                    danger_text("warning: line contains only NUL character");
                    delete buffer;
                    // set empty result
                    result->buffer = nullptr;
                    result->size = 0;
                    return true;
                }

                // shift buffer pointer
                p += len;

                if (p[-1] != '\n') {
                    // no all line in buffer, increase buffer
                    size_t off = p - buffer; // save offset value
                    bufferLen += increaseBufferValue;
                    size_t newAmount = bufferLen * typeSizeof;
                    T *newBuffer = (T *) std::realloc(buffer, newAmount);
                    if (newBuffer == NULL) {
                        delete buffer;
                        throw std::runtime_error(std::strerror(errno));
                    }
                    buffer = newBuffer;
                    p = buffer + off; // restore offset
                    *p = '\0'; // set NUL
                    end = buffer + bufferLen; // change end
                } else {
                    ++lineNumber;
                    if (removeNewLineSymbol) {
                        // TODO: таким образом строка будет содержать
                        // два завершающих NUL
                        p[-1] = '\0';

                        // NOT Windows specific code. If OS not Windows and
                        // CRLF is present then delete CR symbol

#if !defined(_WIN32)
                        /* not windows code */
                        if ((p - buffer) > 1 && p[-2] == '\r') {
                            // три завершающих NUL
                            p[-2] = '\0';
                        }
# endif
                    }

                    result->buffer = buffer;
                    result->size = bufferLen;
                    return true;
                }
            }

            // check error
            if (!std::feof(fp)) {
                // error occurs
                danger_text("error: some error occurs");
                delete buffer;
                throw std::runtime_error(std::strerror(errno));
            }

            // end of file
            delete buffer;
            result->buffer = nullptr;
            result->size = 0;
            return false;
        }

    private:
        FILE *fp;
        size_t typeSizeof; // saved value sizeof
        size_t lineNumber;
        bool isEnd; // end of file
    };
}


#endif