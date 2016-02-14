/**
 * Построение проекций Саммона
 */

#ifndef SRC_KOHONEN_SAMMONMAP_H
#define SRC_KOHONEN_SAMMONMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>
#include <math.h>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "utils/CArrayList.h"
#include "utils/ArrayUtils.h"
#include "file/stream/StreamReader.h"
#include "kohonen/alphafunc/AlphaFunction.h"
#include "kohonen/neighadap/NeighborAdaptation.h"
#include "models/DataSample.h"
#include "utils/console_colors.h"

namespace kohonen {

    template<typename Out>
    class SammonMap {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;
        typedef typename utils::RMatrix<models::NeuronInfo, Out>::Row Neuron;
        typedef utils::ArrayUtils<Out, Out, Out> ArrayUtils;

        SammonMap() {
            randomEngine = new RandomGenerator();
        }

        ~SammonMap() {
            delete randomEngine;
        }

        /**
         * Построение проекций Саммона
         */
        OutCodes* buildMap(OutCodes *trainedSom, size_t iterationLen) {
            long nLen = trainedSom->getRowSize();
            utils::CArrayList<Out> x(nLen);
            utils::CArrayList<Out> y(nLen);
            utils::CArrayList<Out> xu(nLen);
            utils::CArrayList<Out> yu(nLen);
            utils::CArrayList<Out> dd(nLen * (nLen - 1) / 2);

            for (size_t i = 0; i < nLen; ++i) {
                x[i] = (Out) (randomEngine->generate() % nLen) / nLen;
                y[i] = (Out) (i) / nLen;
            }

            size_t mutualIndex = 0;
            size_t colSize = trainedSom->getColSize();
            for (size_t i = 0; i < trainedSom->getRowSize(); ++i) {
                Neuron &ni = trainedSom->getRow(i);
                ni.data.skipped = false; // сбросить флаг в начале
                for (size_t j = 0; j < trainedSom->getRowSize(); ++j) {
                    if (i==j) {
                        break;
                    }
                    Neuron &nj = trainedSom->getRow(j);
                    Out dist = ArrayUtils::euclideanDistance(
                            ni.points, nj.points, colSize);
                    dd[mutualIndex] = dist;
                    mutualIndex++;

                    if (dist == 0) {
                        // нейроны идентичны
                        ni.data.skipped = true;
                        danger_text("skip identical neuron");
                        std::cout << "Neuron " << i << " equal neuron " << j <<
                        std::endl;
                        // выйдем из цикла и начнем новую итерацию верхнего
                        // цикла
//                        break;
                    }
                }
            }

            for (size_t i = 0; i < iterationLen; ++i) {

                for (size_t j = 0; j < nLen; ++j) {
                    Out e1x = 0, e1y = 0, e2x = 0, e2y = 0;
                    for (size_t k = 0; k < nLen; ++k) {
                        if (j != k) {
                            Out xd = x[j] - x[k];
                            Out yd = y[j] - y[k];
                            // TODO: непонятно почему к double приводится только
                            // первый слагаемый - если поставить (double)
                            // ко второму то результат будет другим
                            Out dpj = (Out) std::sqrt((double) xd * xd + yd * yd);

                            /* calculate derivatives */
                            Out dt;
                            if (k > j) {
                                dt = dd[k * (k - 1) / 2 + j];
                            } else {
                                dt = dd[j * (j - 1) / 2 + k];
                            }
                            Out dq = dt - dpj;
                            Out dr = dt * dpj;
                            e1x += xd * dq / dr;
                            e1y += yd * dq / dr;
                            e2x += (dq - xd * xd * (1.0 + dq / dpj) / dpj) / dr;
                            e2y += (dq - yd * yd * (1.0 + dq / dpj) / dpj) / dr;
                        }
                    }
                    /* Correction */
                    xu[j] = x[j] + MAGIC_NUM * e1x / std::fabs(e2x);
                    yu[j] = y[j] + MAGIC_NUM * e1y / std::fabs(e2y);
                }

                /* Move the center of mass to the center of picture */
                Out xx = 0, yy = 0;
                for (size_t j = 0; j < nLen; ++j) {
                    xx += xu[j];
                    yy += yu[j];
                }
                xx /= nLen;
                yy /= nLen;
                for (size_t j = 0; j < nLen; ++j) {
                    x[j] = xu[j] - xx;
                    y[j] = yu[j] - yy;
                }

                /* Error in distances */
                Out e = 0, tot = 0;
                mutualIndex = 0;
                for (size_t j = 1; j < nLen; ++j) {
                    for (size_t k = 0; k < j; ++k) {
                        Out dist = dd[mutualIndex];
                        tot += dist;
                        Out xd = x[j] - x[k];
                        Out yd = y[j] - y[k];
                        // TODO: непонятно почему к double приводится только
                        // первый слагаемый - если поставить (double)
                        // ко второму то результат будет другим
                        Out ee = dist - (Out) std::sqrt((double) xd * xd + yd * yd);
                        e += (ee * ee / dist);
                        mutualIndex++;
                    }
                }

                e /= tot;
//                std::cout << "Mapping error " << e << std::endl;
            }

            OutCodes* sammonCodes = new OutCodes(nLen, 2);
            for (size_t i = 0; i < nLen; ++i) {
                Out points[2];
                points[0] = x[i];
                points[1] = y[i];
                sammonCodes->writeRow(i, points);
            }
            return sammonCodes;
        }

        RandomGenerator *getRandomGenerator() const {
            return randomEngine;
        }

    private:
        // генератор псевдо случайных чисел
        RandomGenerator *randomEngine;

        const double MAGIC_NUM = 0.2;

        /**
         * Для идентичных нейронов (евклидовое расстояние между котороми
         * = 0) установить флаг skipped = true.
         */
        void setSkippedIdenticalNeurons(OutCodes *trainedSom) {
            size_t colSize = trainedSom->getColSize();
            for (size_t i = 0; i < trainedSom->getRowSize(); ++i) {
                Neuron &ni = trainedSom->getRow(i);
                ni.data.skipped = false; // сбросить флаг в начале
                for (size_t j = i + 1; j < trainedSom->getRowSize(); ++j) {
                    Neuron &nj = trainedSom->getRow(j);
                    if (ArrayUtils::euclideanDistance(
                            ni.points, nj.points, colSize) == 0) {
                        ni.data.skipped = true;
                        break;
                    }
                }
            }
        }
    };
}


#endif
