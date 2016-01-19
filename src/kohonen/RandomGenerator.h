/**
 * Simple utils for random generate numbers
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
        }

        ~RandomGenerator() {
        }

        static int generate() {
            return ((int) ((nextValue = (nextValue * 23) % 100000001) %
                           RANDOM_MAX_VALUE));
        }

        static void initGenerator() {
            nextValue = (int) time(NULL);
        }

    private:
        const static long RANDOM_MAX_VALUE = 32767L;
        static unsigned long nextValue;
    };
}




#endif
