#ifndef TEST_COURSERA_COURSERATEST_H
#define TEST_COURSERA_COURSERATEST_H

#include <cerrno>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include <exception>
#include <iostream>
#include <regex.h>
#include <stdexcept>
#include <string>

#include "test.h"

#include "file/CsvFileReader.h"
#include "file/CsvFileRowParser.h"
#include "file/CsvFileWriter.h"
#include "file/LineFileReader.h"
#include "file/stream/CsvFileStreamReader.h"

#include "utils/ArrayUtils.h"
#include "utils/CArrayList.h"
#include "utils/RMatrix.h"
#include "utils/SMatrix.h"

#include "ml/MlUtils.h"

namespace test {
namespace coursera {
void coursera_test();

enum SEX { NO, MALE, FEMALE };

struct Titanic {
    int PassengerId;
    int Survived;
    int Pclass;
    char Name[256];
    SEX Sex; // 0 - not set, 1 - male, 2 - female
    int Age;
    int SibSp;
    int Parch;
    char Ticket[200];
    double Fare;
    char Cabin[200];
    char Embarked[1];
};

class TitanicCsvFileRowParser : public file::CsvFileRowParser<Titanic> {

    bool parseRow(Titanic &row, models::DataSample *samples,
                  file::CsvFileReader *reader) {

        size_t i = 0;
        reader->read(row.PassengerId);

        // Survived - 0
        readNextDataSample(samples[i], reader);
        row.Survived = samples[i].value;
        i++;

        // Pclass - 1
        readNextDataSample(samples[i], reader);
        row.Pclass = samples[i].value;
        i++;

        reader->read(row.Name, 256, true);

        // Sex - 2
        char sex[7];
        size_t numReads = reader->read(sex, 7);
        if (numReads > 0) {
            if (sex[0] == 'm') {
                row.Sex = MALE;
                samples[i].value = 0;
            } else if (sex[0] == 'f') {
                row.Sex = FEMALE;
                samples[i].value = 1;
            } else {
                row.Sex = NO;
                samples[i].skipped = true;
            }
        } else {
            row.Sex = NO;
            samples[i].skipped = true;
        }
        i++;

        // Age - 3
        readNextDataSample(samples[i], reader);
        row.Age = samples[i].value;
        i++;

        // SibSp - 4
        readNextDataSample(samples[i], reader);
        row.SibSp = samples[i].value;
        i++;

        // Parch - 5
        readNextDataSample(samples[i], reader);
        row.Parch = samples[i].value;
        i++;

        return true;
    }

    void initReadFile(file::CsvFileReader *csvReader) {
        // skip first line
        csvReader->toEndLine();
    }

private:
    size_t readNextDataSample(models::DataSample &sample,
                              file::CsvFileReader *csvReader) {
        char buffer[64];
        *buffer = '\0';
        size_t bytesRead = csvReader->read(buffer, 64);
        if (bytesRead == 0) {
            sample.skipped = true;
        } else {
            sample.skipped = false;
            sample.value = std::atof(buffer);
        }
        return bytesRead;
    }
};
} // namespace coursera
} // namespace test

#endif
