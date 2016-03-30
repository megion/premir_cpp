#ifndef SRC_KOHONEN_ALPHAFUNC_LINEARALPHAFUNCTION_H
#define SRC_KOHONEN_ALPHAFUNC_LINEARALPHAFUNCTION_H

#include <cstdio>
#include <iostream>

#include "AlphaFunction.h"

namespace kohonen {
    namespace alphafunc {

        /**
         * Функция скорости обучения линейная (по умолчанию)
         *
         * a(t) = a(0)(1.0 - t/rlen), где C=rlen/100.0
         */
        class LinearAlphaFunction : public AlphaFunction {
        public:
            LinearAlphaFunction() {
            }

            double calcAlpha(size_t index, size_t teachSize, double alpha) {
                return (alpha * (double) (teachSize - index) / (double) teachSize);
            }

        };
    }
}


#endif
