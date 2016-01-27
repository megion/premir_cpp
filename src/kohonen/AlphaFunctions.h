/**
 * Альфа функции используемые при обученнии карт SOM
 */

#ifndef SRC_KOHONEN_ALPHAFUNCTIONS_H
#define SRC_KOHONEN_ALPHAFUNCTIONS_H

#include <stdio.h>

namespace kohonen {

    template<typename T>
    class AlphaFunctions {

    public:

        /**
         * linearly decreasing alpha
         */
        static T linearAlpha(size_t index, size_t teachSize, T alpha) {
            return (alpha * (T) (teachSize - index) / (T) teachSize);
        }

        static T inverseTAlpha(size_t index, size_t teachSize, T alpha) {
            // INV_ALPHA_CONSTANT 100.0
            T c = (T) teachSize / (T) 100;
            return (alpha * c / (c + index));
        }

    private:
        AlphaFunctions() {
        }

    };
}

#endif
