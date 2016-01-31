#ifndef SRC_KOHONEN_ALPHAFUNC_ALPHAFUNCTION_H
#define SRC_KOHONEN_ALPHAFUNC_ALPHAFUNCTION_H

#include <cstdio>

namespace kohonen {
    namespace alphafunc {

        /**
         * Функция скорости обучения
         */
        template<typename T>
        class AlphaFunction {
        public:

            virtual T calcAlpha(size_t index, size_t teachSize, T alpha) = 0;

        };
    }
}


#endif
