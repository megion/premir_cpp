#ifndef SRC_KOHONEN_ALPHAFUNC_ALPHAFUNCTION_H
#define SRC_KOHONEN_ALPHAFUNC_ALPHAFUNCTION_H

#include <cstdio>

namespace kohonen {
    namespace alphafunc {

        /**
         * Функция скорости обучения
         */
        class AlphaFunction {
        public:

            virtual double calcAlpha(size_t index, size_t teachSize, double alpha) = 0;

        };
    }
}


#endif
