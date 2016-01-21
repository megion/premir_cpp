#ifndef SRC_MATRIX_GRAMSCHMIDTSTREAM_H
#define SRC_MATRIX_GRAMSCHMIDTSTREAM_H


#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "utils/CArrayList.h"
#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"

namespace matrix {

    /**
     * Нахождение ортигонального векторного базиса по методу Грама — Шмидта
     * (последовательность векторов h(i)) в соответствии с рекурсивным
     * правилом
     * h(i) = x(i) - sum( ((x(i) * h(j)) / (pow(|h(j)|, 2))) * h(j) )
     * where j = 0 ... i-1
     *
     * Вектора вычисляются потоково т.е. один за другим, поэтому не обязательно
     * иметь доступным сразу все вектора для вычисления
     * для них ортонормированных
     */
    template<typename In, typename Out>
    class GramSchmidtStream {
    public:
        GramSchmidtStream(size_t _inVectorSize) :
                inVectorSize(_inVectorSize) {
            outVectors = new utils::SMatrix<Out>(0, inVectorSize);
            squaredNormsCache = new utils::CArrayList<Out>();
        }

        ~GramSchmidtStream() {
            delete outVectors;
            delete squaredNormsCache;
        }

        utils::SMatrix<Out> *getOutVectors() const {
            return outVectors;
        }

        bool nextVector(const In *inArray) {
            if (outVectors->getRowSize() == 0) {
                // пустой поэтому вычислим квадрат нормы входного массива
                Out inNormSquared = utils::ArrayUtils<In, In, Out>::
                euclideanSquaredNorm(inArray, inVectorSize);
                // if != 0 then init first out value
                if (inNormSquared != 0) {
                    outVectors->pushRow(inArray);
                    squaredNormsCache->push(inNormSquared);
                    return true;
                }
            } else {
                // calculate sum by previous calculated orthogonal vectors
                Out tempSum[inVectorSize];
                for (size_t i = 0; i < outVectors->getRowSize(); ++i) {
                    Out *h = outVectors->getRow(i);
                    Out dist = utils::ArrayUtils<In, Out, Out>::
                    scalarMultiplication(inArray, h, inVectorSize);
                    Out squaredNorm = (*squaredNormsCache)[i];
                    Out k = dist / squaredNorm;

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
                utils::ArrayUtils<Out, In, Out>::
                reverseMinus(tempSum, inArray, inVectorSize);

                Out squaredLastNorm = utils::ArrayUtils<Out, Out, Out>::
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
        utils::SMatrix<Out> *outVectors; // выходной массив векторов
        utils::CArrayList<Out> *squaredNormsCache; // массив посчитанных норм
    };

}

#endif
