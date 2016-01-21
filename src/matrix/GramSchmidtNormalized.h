#ifndef SRC_MATRIX_GRAMSCHMIDTNORMOLIZED_H
#define SRC_MATRIX_GRAMSCHMIDTNORMOLIZED_H


#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"

namespace matrix {

    /**
     * Нахождение ортигонального векторного базиса по методу Грама — Шмидта
     * Вычисляется сразу вся переданая матрица. При этом результирующие вектора
     * хранятся в нормализованном виде.
     *
     * Для экономии памяти для хранения вычисленых векторов
     * можно использовать функцию запоминающую результат во входную матрицу
     * (однако это можно делать только в том случае если типы входной
     * и результрующей матрицы совпадают)
     */
    template<typename In, typename Out>
    class GramSchmidtNormalized {
    public:
        GramSchmidtNormalized() {
        }

        ~GramSchmidtNormalized() {
        }

        /**
         * Вычисление ортогональных векторов для указанной матрицы dataMatrix.
         * Результат будет записан в туже матрицу dataMatrix.
         * Можно применять эту функцию только если тип In == Out
         */
        void gramSchmidt(utils::SMatrix<In> &dataMatrix) {
            size_t rowSize = dataMatrix.getRowSize();
            size_t colSize = dataMatrix.getColSize();
            utils::SMatrix<Out> outMatrix(rowSize, colSize);

            calculateGramSchmidt(dataMatrix, outMatrix);

            // copy result to dataMatrix
            for (size_t r = 0; r < rowSize; ++r) {
                dataMatrix.writeRow(r, outMatrix.getRow(r));
            }
        }

        void gramSchmidt(const utils::SMatrix<In> &inMatrix,
                         utils::SMatrix<Out> &outMatrix) {
            size_t rowSize = inMatrix.getRowSize();
            size_t colSize = inMatrix.getColSize();

            calculateGramSchmidt(inMatrix, outMatrix);
        }

    private:

        void calculateGramSchmidt(const utils::SMatrix<In> &inMatrix,
                                  utils::SMatrix<Out> &outMatrix) {
            size_t rowSize = inMatrix.getRowSize();
            size_t colSize = inMatrix.getColSize();

            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    Out sum = inMatrix(r, c);

                    for (size_t j = 0; j < r; ++j) {
                        for (size_t p = 0; p < colSize; ++p) {
                            sum -= outMatrix(j, c) * outMatrix(j, p) *
                                   inMatrix(r, p);
                        }
                    }

                    outMatrix(r, c) = sum;
                }

                utils::ArrayUtils<Out, Out, Out>::normalization(
                        outMatrix.getRow(r),
                        colSize);
            }
        }
    };

}

#endif
