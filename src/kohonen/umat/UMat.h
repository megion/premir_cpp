#ifndef SRC_KOHONEN_UMAT_UMAT_H
#define SRC_KOHONEN_UMAT_UMAT_H

#include <cstdio>

namespace kohonen {
    namespace umat {

        /**
         * Построение U-матрицы для SOM. Вычисление зависит от топологии карты (HEXA или RECT).
         */
        template<typename C, typename T>
        class UMat {
        public:

            /**
             * Передаваемые координаты bx, by, tx, ty
             * должны быть знаковыми т.к. при вычислении расстояния
             * разность может быть отрицательной.
             */
            virtual T createUMatrix(long xdim, long ydim, long dim, utils::R3DMatrix<bool, C, T>& mvalue) = 0;


        };
    }
}


#endif
