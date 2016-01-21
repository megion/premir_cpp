/**
 * Некоторые функции работы с матрицами
 */

#ifndef SRC_MATRIX_MATRIXUTILS_H
#define SRC_MATRIX_MATRIXUTILS_H

#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"


namespace matrix {

    template<typename InA, typename InB, typename Out>
    class MatrixUtils {

    public:

        /**
         * Нормализация векторов-строчек указанной матрицы
         */
        static void normalizationMatrixRows(utils::SMatrix<InA> &inMatrix) {
            for (size_t r = 0; r < inMatrix.getRowSize(); ++r) {
                InA *row = inMatrix.getRow(r);
                utils::ArrayUtils<InA, InA, InA>::
                normalization(row,
                              inMatrix.getColSize());
            }
        }

    private:
        MatrixUtils() {
        }

    };
}

#endif
