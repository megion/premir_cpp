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

    private:
        size_t inVectorSize; // размер входного массива
        utils::SMatrix<R> *outVectors; // выходной массив векторов
        utils::CArrayList<R> *squaredNormsCache; // массив посчитанных норм
    };

}

#endif
