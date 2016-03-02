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

#include "utils/RMatrix.h"
#include "utils/SMatrix.h"
#include "utils/ArrayUtils.h"
#include "matrix/GramSchmidtNormalized.h"
#include "RandomGenerator.h"
#include "utils/console_colors.h"
#include "file/stream/StreamReader.h"
#include "file/CsvFileSummary.h"
#include "models/models.h"

namespace kohonen {

    template<typename InRow, typename InNum, typename Out>
    class NetworkInitializer {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;

        NetworkInitializer(file::stream::StreamReader<InRow, InNum> *streamReader,
                           file::CsvFileSummary<InRow, InNum> *_summary)
                : dataReader(streamReader), summary(_summary) {
            randomEngine = new RandomGenerator();
        }

        ~NetworkInitializer() {
            delete randomEngine;
        }

        /**
         * Случайная инициализация нейронной сети.
         * TODO: не реализовано.
         */
        OutCodes *randomInitialization() {
            return nullptr;
        }

        RandomGenerator *getRandomGenerator() const {
            return randomEngine;
        }

        /**
         * Линейная инициализация нейронной сети.
         * Инициализация осуществляется вдоль двух главных осей распределния
         * обрабатываемых данных.
         *
         * Инициализацию можно повторить несколько раз, а потом запустить тест
         * позволяющий выбрать лучшую карту (файл с опорными векторами) для
         * процесса обучения.
         *
         * Вначале определяются два собственных вектора с наибольшими
         * собствннными значениями. Затем определяется двумерная матрица
         * центр тяжести которой совпадает со средним значением входных данных.
         * Значения данной матрицы это начальные значения нейроной сети m_i_(0).
         *
         * Полсле этого можно продолжать обучение с фазы сходимости.
         *
         */
        OutCodes *lineInitialization(size_t xdim, size_t ydim, size_t colSize, size_t rowsLimit = 0) {
            utils::CArrayList<models::ColSummary<Out>> stats = (*summary->getSummary());
            utils::SMatrix<Out> *eigens = findEigenVectors(2, colSize, rowsLimit);
            Out *eigen1 = eigens->getRow(0);
            Out *eigen2 = eigens->getRow(1);

            // число нейронов
            size_t neuralNumber = xdim * ydim;
            OutCodes *resultsMatrix = new OutCodes(neuralNumber, colSize);
            for (size_t r = 0; r < resultsMatrix->getRowSize(); ++r) {
                Out xf = 4.0 * (Out) (r % xdim) / (xdim - 1.0) - 2.0;
                Out yf = 4.0 * (Out) (r / xdim) / (ydim - 1.0) - 2.0;

                for (size_t c = 0; c < resultsMatrix->getColSize(); ++c) {
                    (*resultsMatrix)(r, c) = stats[c].average + xf * eigen1[c] + yf * eigen2[c];
                }
            }

            delete eigens;
            return resultsMatrix;
        }


    private:
        // поток входных данных
        file::stream::StreamReader<InRow, InNum> *dataReader;
        file::CsvFileSummary<InRow, InNum> *summary;
        // генератор псевдо случайных чисел
        RandomGenerator *randomEngine;

        /**
         * Найти два собственных вектора с наибольшими
         * собствннными значениями.
         */
        utils::SMatrix<Out> *findEigenVectors(size_t eigenVectorsCount, size_t colSize, size_t rowsLimit) {
            utils::CArrayList<models::ColSummary<Out>> stats = (*summary->getSummary());

            // квадратная матрица
            utils::SMatrix<Out> squareMatrix(colSize, colSize);
            for (size_t row = 0; row < squareMatrix.getRowSize(); ++row) {
                for (size_t col = 0; col < squareMatrix.getColSize(); ++col) {
                    squareMatrix(row, col) = 0;
                }
            }

            // установить поток на начало
            dataReader->rewindReader();

            // iterate by all input matrix elements
            // получение треугольной квадратной матрицы ковариации
            models::DataSample<InNum> samples[colSize];
            InRow rowData;
            size_t rowIndex = 0;
            while (dataReader->readNext(rowData, samples) && (rowsLimit==0 || (rowIndex<rowsLimit))) {
                for (size_t i = 0; i < colSize; ++i) {
                    if (samples[i].skipped) {
                        continue;
                    }
                    for (size_t j = i; j < colSize; ++j) {
                        if (!samples[j].skipped) {
                            squareMatrix(i, j) = squareMatrix(i, j) + (samples[i].value - stats[i].average) *
                                                                      (samples[j].value - stats[j].average);
                        }
                    }
                }
                rowIndex++;
            }

            // получить симметричную матрицу, при этом разделим на число строк входной матрицы
            for (size_t i = 0; i < colSize; ++i) {
                for (size_t j = i; j < colSize; ++j) {
                    squareMatrix(i, j) /= stats[i].count;
                    squareMatrix(j, i) = squareMatrix(i, j);
                }
            }

            // матрица из двух векторов заполненая случайными нормализованными значениями
            utils::SMatrix<Out> uVectors(eigenVectorsCount, colSize);
            Out mu[eigenVectorsCount]; // два собственных значения
            for (size_t i = 0; i < uVectors.getRowSize(); ++i) {
                Out *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    // TODO: генерация рандомных чисел для правильного
                    // вычисления отогональных векторов Грамма-Шмитда:
                    // т.е. каждая строка должна быть различна
                    row[j] = randomEngine->generate() / 16384.0 - 1.0;
                }
                utils::ArrayUtils<Out, Out, Out>::normalization(row, uVectors.getColSize());
                mu[i] = 1.0;
            }

            matrix::GramSchmidtNormalized<Out, Out> gramSchmidtCalc;
            utils::SMatrix<Out> vVectors(eigenVectorsCount, colSize);
            for (int s = 0; s < 10; ++s) {
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < vVectors.getColSize(); ++j) {
                        Out su = utils::ArrayUtils<Out, Out, Out>::
                        scalarMultiplication(squareMatrix.getRow(j), uVectors.getRow(i), uVectors.getColSize());
                        vVectors(i, j) = mu[i] * su + uVectors(i, j);
                    }
                }

                gramSchmidtCalc.gramSchmidt(vVectors);

                Out sum = 0;
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < vVectors.getColSize(); ++j) {
                        Out su = utils::ArrayUtils<Out, Out, Out>::
                        scalarMultiplication(squareMatrix.getRow(j), vVectors.getRow(i), vVectors.getColSize());
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
                throw std::logic_error("error findEigenVectors: mu[0]==0 || mu[1]==0");
            }

            // result - матрица в которой строки это собсвенные вектора
            utils::SMatrix<Out> *result = new utils::SMatrix<Out>(eigenVectorsCount, colSize);
            // поделим каждое значение uVectors на mu[i] и запишем результат в матрицу результатов
            for (size_t i = 0; i < eigenVectorsCount; ++i) {
                Out *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    row[j] /= std::sqrt(mu[i]);
                }
                result->writeRow(i, row);
            }

            return result;
        }

    };
}


#endif
