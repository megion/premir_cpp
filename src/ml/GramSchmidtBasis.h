#ifndef SRC_ML_GRAMSCHMIDTBASIS_H
#define SRC_ML_GRAMSCHMIDTBASIS_H


#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "utils/CArrayList.h"
#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"
#include "utils/matrix_utils.h"

namespace ml {

    /**
     * Нахождение ортигонального векторного базиса по методу Грама — Шмидта
     * (последовательность векторов h(i)) в соответствии с рекурсивным
     * правилом
     * h(i) = x(i) - sum( ((x(i) * h(j)) / (pow(|h(j)|, 2))) * h(j) )
     * where j = 0 ... i-1
     *
     */
    template<typename T, typename R>
    class GramSchmidtBasis {
    public:
        GramSchmidtBasis(size_t _inVectorSize) :
                inVectorSize(_inVectorSize) {
            outVectors = new utils::SMatrix<R>(0, inVectorSize);
            squaredNormsCache = new utils::CArrayList<R>();
        }

        ~GramSchmidtBasis() {
            delete outVectors;
            delete squaredNormsCache;
        }

        utils::SMatrix<R> *getOutVectors() {
            return outVectors;
        }

        bool pushInVector(T *inArray) {
            if (outVectors->getRowSize() == 0) {
                // пустой поэтому вычислим квадрат нормы входного массива
                R inNormSquared = utils::ArrayUtils<T, T, R>::
                euclideanSquaredNorm(inArray, inVectorSize);
                // if != 0 then init first out value
                if (inNormSquared != 0) {
                    outVectors->pushRow(inArray);
                    squaredNormsCache->push(inNormSquared);
                    return true;
                }
            } else {
                // calculate sum by previous calculated orthogonal vectors
                R tempSum[inVectorSize];
                for (size_t i = 0; i < outVectors->getRowSize(); ++i) {
                    R *h = outVectors->getRow(i);
                    R dist = utils::ArrayUtils<T, R, R>::
                    scalarMultiplication(inArray, h, inVectorSize);
                    R squaredNorm = (*squaredNormsCache)[i];
                    R k = dist / squaredNorm;

                    if (i == 0) {
                        for (size_t j = 0; j < inVectorSize; ++j) {
                            tempSum[j] = h[j] * k;
                        }
                    } else {
                        for (size_t j = 0; j < inVectorSize; ++j) {
                            tempSum[j] += h[j] * k;
                        }
                    }
                }

                // tempSum = inArray - tempSum
                // a = b - a
                utils::ArrayUtils<R, T, R>::
                reverseMinus(tempSum, inArray, inVectorSize);

                R squaredLastNorm = utils::ArrayUtils<R, R, R>::
                euclideanSquaredNorm(tempSum, inVectorSize);

                if (squaredLastNorm != 0) {
                    outVectors->pushRow(tempSum);
                    squaredNormsCache->push(squaredLastNorm);
                    return true;
                }
            }

            return false;
        }

        // gram_schmidt(v, n, 2);
        void gram_schmidt2(utils::SMatrix<T> &dataMatrix) {
//            T sum;
//            T w[n*e];

            // n == colSize, e == rowSize

            size_t rowSize = dataMatrix.getRowSize();
            size_t colSize = dataMatrix.getColSize();
            utils::SMatrix<T> wMatrix(rowSize, colSize);

            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    T sum = dataMatrix(r, c);

                    for (size_t j = 0; j < r; ++j) {
                        for (size_t p = 0; p < colSize; ++p) {
                            sum -= wMatrix(j, c) * wMatrix(j, p) *
                                   dataMatrix(r, p);
                        }
                    }

                    wMatrix(r, c) = sum;
                }

                utils::ArrayUtils<T, T, R>::normalization(wMatrix.getRow(r),
                                                          colSize);
            }

//            for (int i=0; i<e; i++) {
//                for (int t=0; t<n; t++) {
//                    T sum=v[i*n+t];
//                    for (int j=0; j<i; j++) {
//                        for (int p=0; p<n; p++) {
//                            sum -= w[j * n + t] * w[j * n + p] * v[i * n + p];
//                        }
//                    }
//
//                    w[i*n+t]=sum;
//                }
//
//                utils::ArrayUtils<T, T, R>::normalization(w+i*n, n);
//            }

            // copy result to dataMatrix
            for (size_t r = 0; r < rowSize; ++r) {
                dataMatrix.writeRow(r, wMatrix.getRow(r));
            }
//            memcpy(v, w, n * e * sizeof(T));
        }

    private:
        size_t inVectorSize; // размер входного массива
        utils::SMatrix<R> *outVectors; // выходной массив векторов
        utils::CArrayList<R> *squaredNormsCache; // массив посчитанных норм
    };

}

#endif
