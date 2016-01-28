/**
 * Генератор псевдослучайных чисел.
 */

#ifndef SRC_KOHONEN_RANDOMGENERATOR_H
#define SRC_KOHONEN_RANDOMGENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace kohonen {

    class RandomGenerator {
    public:

        RandomGenerator() {
            nextValue = 1;
        }

        ~RandomGenerator() {
        }

        int generate() {
            return ((int) ((nextValue = (nextValue * 23) % 100000001) %
                           RANDOM_MAX_VALUE));
        }

        void initGenerator() {
            nextValue = (int) time(NULL);
        }

        void setNextValue(unsigned long _nextValue) {
            nextValue = _nextValue;
        }

    private:
        const static long RANDOM_MAX_VALUE = 32767L;
        unsigned long nextValue;
    };
}




#endif
