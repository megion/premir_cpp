#ifndef SRC_FILE_CSVFILEWRITER_H
#define SRC_FILE_CSVFILEWRITER_H

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

    class CsvFileWriter {
    public:
        CsvFileWriter(const char *filename) {
            fp = fopen(filename, "w");
            if (fp == NULL) {
                std::string errMsg(std::strerror(errno));
                errMsg = errMsg + ": " + filename;
                throw std::runtime_error(errMsg);
            }
        }

        CsvFileWriter(const CsvFileWriter &) = delete;
        CsvFileWriter(CsvFileWriter &&) = delete;
        CsvFileWriter &operator=(const CsvFileWriter &) = delete;
        CsvFileWriter &operator=(CsvFileWriter &&) = delete;

        ~CsvFileWriter() {
            close();
        }

        void close() {
            if (fp) {
                if (fclose(fp) < 0) {
                    throw std::runtime_error(std::strerror(errno));
                }
                fp = nullptr;
            }
        }

        void endLine() {
            std::fprintf(fp, "\n");
        }

        void write(double &v) {
            std::fprintf(fp, "%g ", v);
        }

        void write(float &v) {
            std::fprintf(fp, "%g ", v);
        }

    private:
        FILE *fp;
//        size_t chSizeof; // saved value sizeof
//        size_t lineNumber;
//        char separator;

    };
}


#endif
