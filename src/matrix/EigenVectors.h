/**
 * Вычисление собственных значений и собаственных векторов матрицы
 */

#ifndef SRC_MATRIX_EIGENVECTORS_H
#define SRC_MATRIX_EIGENVECTORS_H

#include <math.h>

#include "utils/SMatrix.h"
#include "utils/console_colors.h"

namespace matrix {

    template<typename T, typename R>
    class EigenVectors {

    public:
        EigenVectors() {
        }

        /**
         * Найти собственные вектора произвольной матрицы
         */
        utils::SMatrix<R>* findEigenVectors(utils::SMatrix<T>& inMatrix) {
        }

    };
}

#endif
