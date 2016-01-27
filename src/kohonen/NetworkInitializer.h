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

    template<typename InStreamReader, typename In, typename Out>
    class NetworkInitializer {
    public:

        NetworkInitializer(InStreamReader *_inStream) : dataReader(
                _inStream) {
        }

        /**
         * Случайная инициализация нейронной сети.
         */
        utils::SMatrix<Out> *randomInitialization() {
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
        utils::SMatrix<Out> *lineInitialization(size_t xdim, size_t ydim) {

            size_t colSize = dataReader->getColSize();

            RandomGenerator::initGenerator();

            utils::SMatrix<Out> *means = findEigenVectors(2);
            Out *mean = means->getRow(0);
            Out *eigen1 = means->getRow(1);
            Out *eigen2 = means->getRow(2);

            // число нейронов
            size_t neuralNumber = xdim * ydim;
            utils::SMatrix<Out> *resultsMatrix = new utils::SMatrix<Out>(
                    neuralNumber, colSize);
            for (size_t r = 0; r < resultsMatrix->getRowSize(); ++r) {
                Out xf = 4.0 * (Out)(r % xdim) / (xdim - 1.0) - 2.0;
                Out yf = 4.0 * (Out)(r / xdim) / (ydim - 1.0) - 2.0;

                for (size_t c = 0; c < resultsMatrix->getColSize(); ++c) {
                    (*resultsMatrix)(r, c) =
                            mean[c] + xf * eigen1[c] + yf * eigen2[c];
                }
            }

            delete means;
            return resultsMatrix;
        }


    private:
        // поток входных данных
        InStreamReader *dataReader;

        /**
         * Найти два собственных вектора с наибольшими
         * собствннными значениями.
         */
        utils::SMatrix<Out> *findEigenVectors(size_t eigenVectorsCount) {
            size_t colSize = dataReader->getColSize();

            // квадратная матрица
            utils::SMatrix<Out> squareMatrix(colSize, colSize);
            for (size_t row = 0; row < squareMatrix.getRowSize(); ++row) {
                for (size_t col = 0; col < squareMatrix.getColSize(); ++col) {
                    squareMatrix(row, col) = 0;
                }
            }

            // colMedians среднее значение по каждой колонке
            Out colMedians[colSize];
            // хранит число считаемых результатов по каждому столбцу
            size_t k2[colSize];
            for (size_t i = 0; i < colSize; ++i) {
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
            In inRow[colSize];
            while (dataReader->readNext(inRow)) {
                for (size_t i = 0; i < colSize; ++i) {
                    colMedians[i] += inRow[i];
                    k2[i]++;
                }
            }

            for (size_t i = 0; i < colSize; ++i) {
                colMedians[i] = colMedians[i] / k2[i];
            }

            // установить поток на начало
            dataReader->rewindReader();

            // iterate by all input matrix elements
            // получение треугольной квадратной матрицы
            while (dataReader->readNext(inRow)) {
                for (size_t i = 0; i < colSize; ++i) {
                    for (size_t j = i; j < colSize; ++j) {
                        squareMatrix(i, j) = squareMatrix(i, j) +
                                             (inRow[i] - colMedians[i]) *
                                             (inRow[j] - colMedians[j]);
                    }
                }
            }

            // поллучить симметричную матрицу, при этом разделим на
            // число строк входной матрицы
            for (size_t i = 0; i < colSize; ++i) {
                for (size_t j = i; j < colSize; ++j) {
                    squareMatrix(i, j) /= k2[i];
                    squareMatrix(j, i) = squareMatrix(i, j);
                }
            }

            // матрица из двух векторов заполненая случайными нормализованными
            // значениями
            utils::SMatrix<Out> uVectors(eigenVectorsCount, colSize);
            Out mu[eigenVectorsCount]; // два собственных значения
            for (size_t i = 0; i < uVectors.getRowSize(); ++i) {
                Out *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    row[j] = RandomGenerator::generate() / 16384.0 - 1.0;
                }
                utils::ArrayUtils<Out, Out, Out>::
                normalization(row, uVectors.getColSize());
                mu[i] = 1;
            }

            // TODO: test
            //  -0.521141, 0.748481, -0.110109, -0.184878, 0.349120,
//          0.019565, 0.369049, 0.620548, 0.384337, -0.575001,
            uVectors(0,0) = -0.521141;
            uVectors(0,1) = 0.748481;
            uVectors(0,2) = -0.110109;
            uVectors(0,3) = -0.184878;
            uVectors(0,4) = 0.349120;

            uVectors(1,0) = 0.019565;
            uVectors(1,1) = 0.369049;
            uVectors(1,2) = 0.620548;
            uVectors(1,3) = 0.384337;
            uVectors(1,4) = -0.575001;

            matrix::GramSchmidtNormalized<Out, Out> gramSchmidtCalc;
            utils::SMatrix<Out> vVectors(eigenVectorsCount, colSize);
            for (int s = 0; s < 10; ++s) {
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < vVectors.getColSize(); ++j) {
                        Out su = utils::ArrayUtils<Out, Out, Out>::
                        scalarMultiplication(squareMatrix.getRow(j),
                                             uVectors.getRow(i),
                                             uVectors.getColSize());
                        vVectors(i, j) = mu[i] * su + uVectors(i, j);
                    }
                }

                gramSchmidtCalc.gramSchmidt(vVectors);

                Out sum = 0;
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < vVectors.getColSize(); ++j) {
                        Out su = utils::ArrayUtils<Out, Out, Out>::
                        scalarMultiplication(squareMatrix.getRow(j),
                                             vVectors.getRow(i),
                                             vVectors.getColSize());
                        sum += std::abs(vVectors(i, j) / su);
                    }
                    mu[i] = sum / colSize;
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
            utils::SMatrix<Out> *result = new utils::SMatrix<Out>(
                    eigenVectorsCount + 1, colSize);
            result->writeRow(0, colMedians);
            // поделим каждое значение uVectors на mu[i] и запишем результат
            // в матрицу результатов
            for (size_t i = 0; i < eigenVectorsCount; ++i) {
                Out *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    row[j] /= std::sqrt(mu[i]);
                }
                result->writeRow(i + 1, row);
            }

            return result;
        }

    };
}


#endif
