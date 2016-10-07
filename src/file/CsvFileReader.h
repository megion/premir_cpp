#ifndef SRC_FILE_CSVFILEREADER_H
#define SRC_FILE_CSVFILEREADER_H

#include <cstdio>
#include <cmath>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <limits>

#include "utils/console_colors.h"

namespace file {

    class CsvFileReader {
    public:
        CsvFileReader(const char *filename, const char sep) :
                separator(sep), chSizeof(sizeof(char)), lineNumber(0),
                isEnd(false), emptyRead(false), strQuote('\"') {
            fp = fopen(filename, "r");
            if (fp == NULL) {
                std::string errMsg(std::strerror(errno));
                errMsg = errMsg + ": " + filename;
                throw std::runtime_error(errMsg);
            }
        }

        CsvFileReader(const CsvFileReader &) = delete;

        CsvFileReader(CsvFileReader &&) = delete;

        CsvFileReader &operator=(const CsvFileReader &) = delete;

        CsvFileReader &operator=(CsvFileReader &&) = delete;

        ~CsvFileReader() {
            close();
        }

        size_t getLineNumber() {
            return lineNumber;
        }

        void setStrQuote(const char quote) {
            strQuote = quote;
        }

        void close() {
            if (fp) {
                if (fclose(fp) < 0) {
                    throw std::runtime_error(std::strerror(errno));
                }
                fp = nullptr;
            }
        }

        bool hasNext() {
            return !isEnd;
        }

        bool isEmptyRead() {
            return emptyRead;
        }

        void toEndLine() {
            int ch;
            for (; ;) {
                ch = fgetc(fp); // wint_t ch = fgetwc(fp)
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
                    return;
                }
            }
        }

        /**
         * Установить в начало текущее положение файла.
         */
        void rewindReader() {
            rewind(fp);
            lineNumber = 0;
            isEnd = false;
            emptyRead = false;
        }

        size_t read(double &v) {
            char buffer[64];
            *buffer = '\0';
            size_t bytesRead = nextValue(buffer, 64);
//            if(bytesRead>0) {
//                v = std::atof(buffer);
//            }
            v = std::atof(buffer);
            return bytesRead;
        }

        size_t read(float &v) {
            char buffer[64];
            *buffer = '\0';
            size_t bytesRead = nextValue(buffer, 64);
            v = std::atof(buffer);
            return bytesRead;
        }

        size_t read(int &v) {
            char buffer[64];
            *buffer = '\0';
            size_t bytesRead = nextValue(buffer, 64);
            v = std::atoi(buffer);
            return bytesRead;
        }

        size_t read(size_t &v) {
            char buffer[64];
            *buffer = '\0';
            size_t bytesRead = nextValue(buffer, 64);
            v = std::atol(buffer);
            return bytesRead;
        }

        size_t read(long &v) {
            char buffer[64];
            *buffer = '\0';
            size_t bytesRead = nextValue(buffer, 64);
            v = std::atol(buffer);
            return bytesRead;
        }

        size_t read(char &v) {
            char buffer[2];
            *buffer = '\0';
            size_t num = nextValue(buffer, 2);
            v = buffer[0];
            return num;
        }

        size_t read(char *v, size_t len) {
            return nextValue(v, len);
        }

        size_t read(char *v, size_t len, bool useStrQuote) {
            return nextValue(v, len, useStrQuote);
        }

    private:
        FILE *fp;
        size_t chSizeof; // saved value sizeof
        size_t lineNumber;
        bool isEnd; // end of file
        bool emptyRead; // no read data
        char separator;
        char strQuote;

        size_t nextValue(char *buffer, size_t bufferSize, bool useStrQuote = false) {
            size_t i = 0;
            int ch;
            for (; ;) {
                ch = fgetc(fp); // wint_t ch = fgetwc(fp)
                if (ch == EOF) {
                    if (feof(fp)) {
                        isEnd = true; // end of file
                    } else {
                        // error occurs
                        danger_text("error: some error occurs");
                        throw std::runtime_error(std::strerror(errno));
                    }
                    if (i == 0) {
                        // end of file and has NO data for reading
                        emptyRead = true;
                    }
                    return i;
                }

                if (i >= bufferSize) {
                    std::cout<<"buffer: "<<buffer<< " size: "<< bufferSize << std::endl;
                    danger_text("buffer is overfull, use dynamic buffer");
                    throw std::runtime_error("local buffer is overfull");
                }
                if (ch == '\n') {
                    /* on unix remove CR if exist */
#if !defined(_WIN32)
                    /* not windows code */
                    if (i > 0 && buffer[i - 1] == '\r') {
                        buffer[i - 1] = '\0';
                    }
# endif
                    buffer[i] = '\0';
                    ++lineNumber;
                    return i;
                }

                if (useStrQuote && ch == strQuote) {
                    for (ch = fgetc(fp); ch!=strQuote && ch != EOF; ++i,
                            ch = fgetc(fp)) {
                        if (i >= bufferSize) {
                            danger_text("buffer is overfull, use dynamic buffer");
                            throw std::runtime_error("local buffer is overfull");
                        }
                        buffer[i] = ch;
                    }
                } else {
                    if (ch == separator) {
                        buffer[i] = '\0';
                        return i;
                    }
                    buffer[i] = ch;
                    ++i;
                }
            }
        }
    };
}


#endif
