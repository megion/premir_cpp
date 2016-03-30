#ifndef SRC_KOHONEN_ALPHAFUNC_INVERSEALPHAFUNCTION_H
#define SRC_KOHONEN_ALPHAFUNC_INVERSEALPHAFUNCTION_H

#include <cstdio>
#include <iostream>

#include "AlphaFunction.h"

namespace kohonen {
    namespace alphafunc {

        /**
         * Функция скорости обучения обратно пропорциональная времени
         *
         * a = a(0)C/(C+t), где C=rlen/100.0
         */
        class InverseAlphaFunction : public AlphaFunction {
        public:
            InverseAlphaFunction() {
            }

            double calcAlpha(size_t index, size_t teachSize, double alpha) {
                // INV_ALPHA_CONSTANT 100.0
                double c = (double) teachSize / (double) 100.0;
                return (alpha * c / (c + index));
            }

        };
    }
}


#endif
