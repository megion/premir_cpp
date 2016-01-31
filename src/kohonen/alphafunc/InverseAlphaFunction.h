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
        template<typename T>
        class InverseAlphaFunction : public AlphaFunction<T> {
        public:
            InverseAlphaFunction() {
            }

            T calcAlpha(size_t index, size_t teachSize, T alpha) {
                // INV_ALPHA_CONSTANT 100.0
                T c = (T) teachSize / (T) 100.0;
                return (alpha * c / (c + index));
            }

        };
    }
}


#endif
