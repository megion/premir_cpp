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
#include <cmath>

#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"
#include "matrix/GramSchmidtNormalized.h"
#include "RandomGenerator.h"
#include "utils/console_colors.h"

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
        utils::SMatrix<R> *lineInitialization(utils::SMatrix<T> &inMatrix,
                                              size_t xdim, size_t ydim) {

            RandomGenerator::initGenerator();

            utils::SMatrix<R> *means = findEigenVectors(inMatrix, 2);
            R *mean = means->getRow(0);
            R *eigen1 = means->getRow(1);
            R *eigen2 = means->getRow(2);

            // число нейронов
            size_t neuralNumber = xdim * ydim;
            utils::SMatrix<R> *resultsMatrix = new utils::SMatrix<R>(
                    neuralNumber, inMatrix.getColSize());
            for (size_t r = 0; r < resultsMatrix->getRowSize(); ++r) {
                R xf = 4.0 * (R) (r % xdim) / (xdim - 1.0) - 2.0;
                R yf = 4.0 * (R) (r / xdim) / (ydim - 1.0) - 2.0;

//                std::cout << "r: " << r << ", xf: " << xf << ", yf: " << yf << std::endl;
                for (size_t c = 0; c < resultsMatrix->getColSize(); ++c) {
                    (*resultsMatrix)(r, c) =
                            mean[c] + xf * eigen1[c] + yf * eigen2[c];
                }
            }

            delete means;
            return resultsMatrix;
        }


    private:

        /**
         * Найти два собственных вектора с наибольшими
         * собствннными значениями.
         */
        utils::SMatrix<R> *findEigenVectors(utils::SMatrix<T> &inMatrix,
                                            size_t eigenVectorsCount) {
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
            // хранит число считаемых результатов по каждому столбцу
            size_t k2[n];
            for (size_t i = 0; i < n; ++i) {
                colMedians[i] = 0;
                k2[i] = 0;
            }

            // iterate by all input matrix elements
            // после этой итерации k2 будет содержать число строк,
            // т.е. одинаковым для каждого столбца. Однако оно может быть
            // различным если внедрить подсчет не каждого значения столбца.
            // (пока такое не реализовано)
            // TODO: т.к. данные могут быть большие то возможно переполнение
            // переменных хранимых в colMedians
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
            utils::SMatrix<R> uVectors(eigenVectorsCount, n);
            R mu[eigenVectorsCount]; // два собственных значения
            for (size_t i = 0; i < uVectors.getRowSize(); ++i) {
                R *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    row[j] = RandomGenerator::generate() / 16384.0 - 1.0;
                }
                utils::ArrayUtils<R, R, R>::
                normalization(row, uVectors.getColSize());
                mu[i] = 1;
            }

            //  -0.521141, 0.748481, -0.110109, -0.184878, 0.349120,
//          0.019565, 0.369049, 0.620548, 0.384337, -0.575001,
//            uVectors(0,0) = -0.521141;
//            uVectors(0,1) = 0.748481;
//            uVectors(0,2) = -0.110109;
//            uVectors(0,3) = -0.184878;
//            uVectors(0,4) = 0.349120;
//
//            uVectors(1,0) = 0.019565;
//            uVectors(1,1) = 0.369049;
//            uVectors(1,2) = 0.620548;
//            uVectors(1,3) = 0.384337;
//            uVectors(1,4) = -0.575001;

            matrix::GramSchmidtNormalized<R, R> gramSchmidtCalc;
            utils::SMatrix<R> vVectors(eigenVectorsCount, n);
            for (int s = 0; s < 10; ++s) {
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < n; ++j) {
                        R su = utils::ArrayUtils<R, R, R>::
                        scalarMultiplication(squareMatrix.getRow(j),
                                             uVectors.getRow(i), n);
                        vVectors(i, j) = mu[i] * su + uVectors(i, j);
                    }
                }

                gramSchmidtCalc.gramSchmidt(vVectors);

                R sum = 0;
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < n; ++j) {
                        R su = utils::ArrayUtils<R, R, R>::
                        scalarMultiplication(squareMatrix.getRow(j),
                                             vVectors.getRow(i), n);
                        sum += std::abs(vVectors(i, j) / su);
                    }
                    mu[i] = sum / n;
                }

                // скопировать значения vVectors в uVectors
                vVectors.copyTo(uVectors);
            }

            if (mu[0] == 0 || mu[1] == 0) {
                // ошибка
                danger_text("error findEigenVectors: mu[0]==0 || mu[1]==0");
                throw std::
                logic_error("error findEigenVectors: mu[0]==0 || mu[1]==0");
            }

            // result - матрица в которой:
            // 1-я строка средние-арифметическое по каждой из колонок
            // 2-я и 3-я строки это собсвенные вектора
            utils::SMatrix<R> *result = new utils::SMatrix<R>(
                    eigenVectorsCount + 1, n);
            result->writeRow(0, colMedians);
            // поделим каждое значение uVectors на mu[i] и запишем результат
            // в матрицу результатов
            for (size_t i = 0; i < eigenVectorsCount; ++i) {
                R *row = uVectors.getRow(i);
                for (size_t j = 0; j < n; ++j) {
                    row[j] /= std::sqrt(mu[i]);
                }
                result->writeRow(i + 1, row);
            }

            return result;
        }

    };
}


#endif
