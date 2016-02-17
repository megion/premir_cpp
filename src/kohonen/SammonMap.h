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

        SammonMap(long _nLen) :
                nLen(_nLen),
                randomEngine(new RandomGenerator()),
                mapPoints(new utils::CArrayList<Point>(nLen, 1, nLen)),
                correctionPoints(new utils::CArrayList<Point>(nLen)),
                dd(new utils::CArrayList<Out>(nLen * (nLen - 1) / 2)) {
        }

        ~SammonMap() {
            delete randomEngine;
            delete mapPoints;
            delete correctionPoints;
            delete dd;
        }

        struct Point {
            Out x;
            Out y;
        };

        void initializeMap(OutCodes *trainedSom) {
            for (size_t i = 0; i < nLen; ++i) {
                (*mapPoints)[i].x = (Out) (randomEngine->generate() % nLen) / nLen;
                (*mapPoints)[i].y = (Out) (i) / nLen;
            }

            size_t mutualIndex = 0;
            size_t colSize = trainedSom->getColSize();
            for (size_t i = 0; i < trainedSom->getRowSize(); ++i) {
                Neuron &ni = trainedSom->getRow(i);
                ni.data.skipped = false; // сбросить флаг в начале
                for (size_t j = 0; j < trainedSom->getRowSize(); ++j) {
                    if (i == j) {
                        break;
                    }
                    Neuron &nj = trainedSom->getRow(j);
                    Out dist = ArrayUtils::euclideanDistance(ni.points, nj.points, colSize);
                    (*dd)[mutualIndex] = dist;
                    mutualIndex++;

                    if (dist == 0) {
                        // нейроны идентичны
                        ni.data.skipped = true;
                        danger_text("skip identical neuron");
                        std::cout << "Neuron " << i << " equal neuron " << j << std::endl;
                        // выйдем из цикла и начнем новую итерацию верхнего
                        // цикла
//                        break;
                    }
                }
            }
        }

        /**
         * Построение проекций Саммона - одна итерация
         */
        void doOneIteration() {
            for (size_t j = 0; j < nLen; ++j) {
                Out e1x = 0, e1y = 0, e2x = 0, e2y = 0;
                for (size_t k = 0; k < nLen; ++k) {
                    if (j != k) {
                        Out xd = (*mapPoints)[j].x - (*mapPoints)[k].x;
                        Out yd = (*mapPoints)[j].y - (*mapPoints)[k].y;
                        // TODO: непонятно почему к double приводится только
                        // первый слагаемый - если поставить (double)
                        // ко второму то результат будет другим
                        Out dpj = (Out) std::sqrt((double) xd * xd + yd * yd);

                        /* calculate derivatives */
                        Out dt;
                        if (k > j) {
                            dt = (*dd)[k * (k - 1) / 2 + j];
                        } else {
                            dt = (*dd)[j * (j - 1) / 2 + k];
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
                (*correctionPoints)[j].x = (*mapPoints)[j].x + MAGIC_NUM * e1x / std::fabs(e2x);
                (*correctionPoints)[j].y = (*mapPoints)[j].y + MAGIC_NUM * e1y / std::fabs(e2y);
            }

            /* Move the center of mass to the center of picture */
            Out xx = 0, yy = 0;
            for (size_t j = 0; j < nLen; ++j) {
                xx += (*correctionPoints)[j].x;
                yy += (*correctionPoints)[j].y;
            }
            xx /= nLen;
            yy /= nLen;
            for (size_t j = 0; j < nLen; ++j) {
                (*mapPoints)[j].x = (*correctionPoints)[j].x - xx;
                (*mapPoints)[j].y = (*correctionPoints)[j].y - yy;
            }

            /* Error in distances */
            Out e = 0, tot = 0;
            size_t mutualIndex = 0;
            for (size_t j = 1; j < nLen; ++j) {
                for (size_t k = 0; k < j; ++k) {
                    Out dist = (*dd)[mutualIndex];
                    tot += dist;
                    Out xd = (*mapPoints)[j].x - (*mapPoints)[k].x;
                    Out yd = (*mapPoints)[j].y - (*mapPoints)[k].y;
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

        /**
         * Построение проекций Саммона
         */
        void buildMap(size_t iterationLen) {
            for (size_t i = 0; i < iterationLen; ++i) {
                doOneIteration();
            }
        }

        RandomGenerator *getRandomGenerator() const {
            return randomEngine;
        }

        utils::CArrayList<Point> *getMapPoints() const {
            return mapPoints;
        }

    private:
        // генератор псевдо случайных чисел
        RandomGenerator *randomEngine;

        const double MAGIC_NUM = 0.2;

        long nLen;
        utils::CArrayList<Point> *mapPoints;
        utils::CArrayList<Point> *correctionPoints;
        utils::CArrayList<Out> *dd;

    };
}


#endif
