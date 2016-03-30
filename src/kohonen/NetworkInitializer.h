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
#include "matrix/GramSchmidtStream.h"
#include "matrix/MatrixUtils.h"
#include "RandomGenerator.h"
#include "utils/console_colors.h"
#include "file/stream/StreamReader.h"
#include "file/CsvFileSummary.h"
#include "models/models.h"

namespace kohonen {

    template<typename InRow>
    class NetworkInitializer {
    public:

        typedef utils::RMatrix<models::NeuronInfo, double> OutCodes;

        NetworkInitializer(file::stream::StreamReader<InRow> *streamReader, file::CsvFileSummary<InRow> *_summary)
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
        OutCodes *lineInitialization(size_t xdim, size_t ydim, size_t colSize, bool isScale, size_t rowsLimit = 0) {
            utils::CArrayList<models::ColSummary> stats = *(summary->getSummary());
            utils::SMatrix<double> *eigens = findEigenVectors(2, colSize, isScale, rowsLimit);
            double *eigen1 = eigens->getRow(0);
            double *eigen2 = eigens->getRow(1);

            // число нейронов
            size_t neuralNumber = xdim * ydim;
            OutCodes *resultsMatrix = new OutCodes(neuralNumber, colSize);
            for (size_t r = 0; r < resultsMatrix->getRowSize(); ++r) {
                double xf = 4.0 * (double) (r % xdim) / (xdim - 1.0) - 2.0;
                double yf = 4.0 * (double) (r / xdim) / (ydim - 1.0) - 2.0;

                for (size_t c = 0; c < resultsMatrix->getColSize(); ++c) {
                    (*resultsMatrix)(r, c) = stats[c].getAverage(isScale) + xf * eigen1[c] + yf * eigen2[c];
                }
            }

            delete eigens;
            return resultsMatrix;
        }


    private:
        // поток входных данных
        file::stream::StreamReader<InRow> *dataReader;
        file::CsvFileSummary<InRow> *summary;
        // генератор псевдо случайных чисел
        RandomGenerator *randomEngine;

        /**
         * Найти два собственных вектора с наибольшими
         * собствннными значениями.
         */
        utils::SMatrix<double> *findEigenVectors(size_t eigenVectorsCount, size_t colSize, bool isScale,
                                              size_t rowsLimit) {
            utils::CArrayList<models::ColSummary> stats = *(summary->getSummary());

            // квадратная матрица
            utils::SMatrix<double> squareMatrix(colSize, colSize);
            for (size_t row = 0; row < squareMatrix.getRowSize(); ++row) {
                for (size_t col = 0; col < squareMatrix.getColSize(); ++col) {
                    squareMatrix(row, col) = 0;
                }
            }

            // установить поток на начало
            dataReader->rewindReader();

            // iterate by all input matrix elements
            // получение треугольной квадратной матрицы ковариации
            models::DataSample samples[colSize];
            InRow rowData;
            size_t rowIndex = 0;
            while (dataReader->readNext(rowData, samples) && (rowsLimit == 0 || (rowIndex < rowsLimit))) {
                // print process
                if(rowIndex!=0) {
                    if (rowIndex%100000==0) {
                        std::cout<<".";
                    }
                    if(rowIndex%1000000==0) {
                        std::cout<<std::endl<<rowIndex<<std::endl;
                    }
                }

//                summary->skipEmptyColSamples(samples);
                if (isScale) {
                    summary->scaleSamples(samples);
                }
                for (size_t i = 0; i < colSize; ++i) {
                    if (samples[i].skipped) {
                        continue;
                    }
                    for (size_t j = i; j < colSize; ++j) {
                        if (!samples[j].skipped) {
                            squareMatrix(i, j) =
                                    squareMatrix(i, j) + (samples[i].value - stats[i].getAverage(isScale)) *
                                                         (samples[j].value - stats[j].getAverage(isScale));
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
            utils::SMatrix<double> uVectors(eigenVectorsCount, colSize);
            double mu[eigenVectorsCount]; // два собственных значения
            for (size_t i = 0; i < uVectors.getRowSize(); ++i) {
                double *row = uVectors.getRow(i);
                for (size_t j = 0; j < uVectors.getColSize(); ++j) {
                    // TODO: генерация рандомных чисел для правильного
                    // вычисления отогональных векторов Грамма-Шмитда:
                    // т.е. каждая строка должна быть различна
                    row[j] = randomEngine->generate() / 16384.0 - 1.0;
                }
                utils::ArrayUtils<double, double, double>::normalization(row, uVectors.getColSize());
                mu[i] = 1.0;
            }

            matrix::GramSchmidtNormalized<double, double> gramSchmidtCalc;

            utils::SMatrix<double> vVectors(eigenVectorsCount, colSize);
            for (int s = 0; s < 10; ++s) {
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < vVectors.getColSize(); ++j) {
                        double su = utils::ArrayUtils<double, double, double>::
                        scalarMultiplication(squareMatrix.getRow(j), uVectors.getRow(i), uVectors.getColSize());
                        vVectors(i, j) = mu[i] * su + uVectors(i, j);
                    }
                }

                gramSchmidtCalc.gramSchmidt(vVectors);

                double sum = 0;
                for (size_t i = 0; i < vVectors.getRowSize(); ++i) {
                    for (size_t j = 0; j < vVectors.getColSize(); ++j) {
                        double su = utils::ArrayUtils<double, double, double>::
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
            utils::SMatrix<double> *result = new utils::SMatrix<double>(eigenVectorsCount, colSize);
            // поделим каждое значение uVectors на mu[i] и запишем результат в матрицу результатов
            for (size_t i = 0; i < eigenVectorsCount; ++i) {
                double *row = uVectors.getRow(i);
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
