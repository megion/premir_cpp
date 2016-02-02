#ifndef TEST_COURSERA_COURSERATEST_H
#define TEST_COURSERA_COURSERATEST_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <regex>

#include "test.h"

#include "file/LineFileReader.h"
#include "file/CsvFileReader.h"
#include "utils/CArrayList.h"
#include "utils/ArrayUtils.h"


namespace test {
    namespace coursera {
        void coursera_test();

        enum SEX {NO, MALE, FEMALE};

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
    }
}

#endif

