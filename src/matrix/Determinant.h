/**
 * Calculate matrix determinant
 */

#ifndef SRC_MATRIX_DETERMINANT_H
#define SRC_MATRIX_DETERMINANT_H

#include "utils/SMatrix.h"

namespace matrix {

    template<typename T, typename R>
    class Determinant {

    public:
        Determinant() {
        }

        /**
         *
         */
        R gaussDeterminant(const utils::SMatrix<T>& matrix) {
            size_t matrixSize = matrix.getColSize();

            // make triangular matrix by gauss algorithm
            // for j = 0 ... N - 2
            //     for i = j + 1 ... N - 1
            //         a(i) = a (i) - (a(i,j)/a(j,j)) * a(j)
            for (size_t j = 0; j<(matrixSize-1); ++j) {
                for (size_t i=j+1; i<matrixSize; ++i) {
                    R r = matrix(i,j)/matrix(j,j);
                    T* aiVec = matrix[i];
                    T* ajVec = matrix[j];
                    // aiVec = ajVec - r * ajVec
                    for (size_t n = j; n<matrixSize; ++n) {
                        aiVec[n] = aiVec[n] - r * ajVec[n];
                    }
                }
            }

            // calculate determinant as
            // d = a(0,0)
            // for i = 1 .. N - 1
            //     d = d * a(i,i)
            R determinant = matrix(0,0);
            for (size_t n = 1; n<matrixSize; ++n) {
                determinant = determinant * matrix(n,n);
            }

            return determinant;
        }

    };
}

#endif
