/**
 * Initialization kohonen neural networks
 */

#ifndef SRC_KOHONEN_NETWORKITIALIZER_H
#define SRC_KOHONEN_NETWORKITIALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <math.h>

#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"
#include "RandomGenerator.h"

namespace kohonen {

    template<typename T, typename R>
    class NetworkInitializer {
    public:

        NetworkInitializer() {
        }

        /**
         * Случайная инициализация нейронной сети.
         */
        utils::SMatrix<R> *randomInitialization(utils::SMatrix<T> &inMatrix) {
            return nullptr;
        }

        /**
         * Линейная инициализация нейронной сети.
         * Вначале определяются два собственных вектора с наибольшими
         * собствннными значениями. Затем определяется двумерная матрица
         * центр тяжести которой совпадает со средним значением входных данных.
         * Значения данной матрицы это начальные значения нейроной сети m_i_(0).
         *
         * Полсле этого можно продолжать обучение с фазы сходимости.
         *
         */
        utils::SMatrix<R> *lineInitialization(utils::SMatrix<T> &inMatrix) {

            RandomGenerator::initGenerator();

            findEigenVectors(inMatrix);

            return nullptr;
        }


    private:

        /**
         * Найти два собственных вектора с наибольшими
         * собствннными значениями.
         */
        utils::SMatrix<R> *findEigenVectors(utils::SMatrix<T> &inMatrix) {
            size_t n = inMatrix.getColSize();

            // квадратная матрица
            utils::SMatrix<R> squareMatrix(n, n);
            for (size_t row = 0; row < squareMatrix.getRowSize(); ++row) {
                for (size_t col = 0; col < squareMatrix.getColSize(); ++col) {
                    squareMatrix(row, col) = 0;
                }
            }

            // colMedians среднее значение по каждой колонке
            R colMedians[n];
            for (size_t i = 0; i < n; ++i) {
                colMedians[i] = 0;
            }

            size_t k2[n];
            for (size_t i = 0; i < n; ++i) {
                k2[i] = 0;
            }

            // iterate by all input matrix elements
            // после этой итерации k2 будет содержать число строк,
            // т.е. одинаковым для каждого столбца. Однако оно может быть
            // различным если внедрить подсчет не каждого значения столбца.
            // (пока такое не реализовано)
            for (size_t k = 0; k < inMatrix.getRowSize(); ++k) {
                T *row = inMatrix.getRow(k);
                for (size_t i = 0; i < n; ++i) {
                    colMedians[i] += row[i];
                    k2[i]++;
                }
            }

            for (size_t i = 0; i < n; ++i) {
                colMedians[i] = colMedians[i] / k2[i];
            }

            // iterate by all input matrix elements
            // получение треугольной квадратной матрицы
            for (size_t k = 0; k < inMatrix.getRowSize(); ++k) {
                T *row = inMatrix.getRow(k);
                for (size_t i = 0; i < n; ++i) {
                    for (size_t j = i; j < n; j++) {
                        squareMatrix(i, j) = squareMatrix(i, j) +
                                             (row[i] - colMedians[i]) *
                                             (row[j] - colMedians[j]);
                    }
                }
            }

            // поллучить симметричную матрицу, при этом разделим на
            // число строк входной матрицы
            size_t inRowSize = inMatrix.getRowSize();
            for (size_t i = 0; i < n; i++) {
                for (size_t j = i; j < n; j++) {
                    squareMatrix(i, j) /= inRowSize;
                    squareMatrix(j, i) = squareMatrix(i, j);
                }
            }

            // матрица из двух векторов заполненая случайными нормализованными
            // значениями
            utils::SMatrix<R> uVectors(2, n);
            R mu[2]; // два собственных значения
            for (size_t i = 0; i < uVectors.getRowSize(); ++i) {
                R *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    row[j] = RandomGenerator::generate() / 16384.0 - 1.0;
                }
                utils::ArrayUtils<R, R, R>::
                normalization(row, uVectors.getColSize());
                mu[i] = 1;
            }

//            uVectors.print();

//            for (size_t i = 0; i < 2; i++) {
//                for (size_t j = 0; j < n; j++) {
//                    printf("%f, ", u[i * n + j]);
//                }
//                printf("\n");
//            }


//            squareMatrix.print();

            utils::SMatrix<R> vVectors(2, n);
            for (int s=0; s<10; ++s) {
                for (size_t i=0; i<vVectors.getRowSize(); ++i) {
                    for (size_t j=0; j<n; ++j) {
                        R su = utils::ArrayUtils<R, R, R>::
                        scalarMultiplication(squareMatrix.getRow(j),
                                             uVectors.getRow(i), n);
                        vVectors(i, j) = mu[i] * su + uVectors(i, j);
                    }
                }


                gramSchmidt2(vVectors);

//                sum=0.0;
//                for (i=0; i<2; i++) {
//                    for (j=0; j<n; j++)
//                        sum+=fabs(v[i*n+j]/dotprod(r+j*n, v+i*n, n));
//
//                    mu[i]=sum/n;
//                }
//
//                memcpy(u, v, 2*n*sizeof(float));
            }

            return nullptr;
        }

        /**
         * Вычисление ортогональных векторов для указанной матрицы dataMatrix.
         * Это подобие ортогонализации Грамма-Шмитда, но не является ей.
         * Для настоящей ортогонализации нужно использовать GramSchmidtBasis.
         * findEigenVectors - использует gramSchmidt2 ровно также как
         * и оригинальный код SOM_PAK. Чем это обусловлено - неизвестно.
         */
        void gramSchmidt2(utils::SMatrix<R> &dataMatrix) {
            size_t rowSize = dataMatrix.getRowSize();
            size_t colSize = dataMatrix.getColSize();
            utils::SMatrix<R> wMatrix(rowSize, colSize);

            for (size_t r = 0; r < rowSize; ++r) {
                for (size_t c = 0; c < colSize; ++c) {
                    R sum = dataMatrix(r, c);

                    for (size_t j = 0; j < r; ++j) {
                        for (size_t p = 0; p < colSize; ++p) {
                            sum -= wMatrix(j, c) * wMatrix(j, p) *
                                   dataMatrix(r, p);
                        }
                    }

                    wMatrix(r, c) = sum;
                }

                utils::ArrayUtils<R, R, R>::normalization(wMatrix.getRow(r),
                                                          colSize);
            }

            // copy result to dataMatrix
            for (size_t r = 0; r < rowSize; ++r) {
                dataMatrix.writeRow(r, wMatrix.getRow(r));
            }
        }

    };
}


#endif
