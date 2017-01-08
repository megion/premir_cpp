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
#include "models/models.h"
#include "utils/console_colors.h"

namespace kohonen {

    class SammonMap {
    public:

        typedef utils::RMatrix<models::NeuronInfo, double> OutCodes;
        typedef typename utils::RMatrix<models::NeuronInfo, double>::Row Neuron;
        typedef utils::ArrayUtils<double, double, double> ArrayUtils;

        SammonMap(long _nLen) :
			nLen(_nLen),
			randomEngine(new RandomGenerator()),
			mapPoints(new utils::CArrayList<Point>(nLen, 1)),
			correctionPoints(new utils::CArrayList<Point>(nLen)),
			dd(new utils::CArrayList<double>(nLen * (nLen - 1) / 2)) {

			mapPoints->setSize(nLen);
        }

        ~SammonMap() {
            delete randomEngine;
            delete mapPoints;
            delete correctionPoints;
            delete dd;
        }

        struct Point {
            double x;
            double y;

            friend std::ostream &operator<<(std::ostream &os, const Point &val) {
                os << val.x << " " << val.y;
                os << std::endl;
                return os;
            }
        };

        void initializeMap(OutCodes *trainedSom) {
            for (size_t i = 0; i < nLen; ++i) {
                (*mapPoints)[i].x = (double) (randomEngine->generate() % nLen) / nLen;
                (*mapPoints)[i].y = (double) (i) / nLen;
            }

            size_t mutualIndex = 0;
            size_t colSize = trainedSom->getColSize();
//            size_t numIdentical = 0;
            for (size_t i = 0; i < trainedSom->getRowSize(); ++i) {
                Neuron &ni = trainedSom->getRow(i);
//                ni.data.skipped = false; // сбросить флаг в начале
                for (size_t j = 0; j < trainedSom->getRowSize(); ++j) {
                    if (i == j) {
                        break;
                    }

                    Neuron &nj = trainedSom->getRow(j);
                    double dist = ArrayUtils::euclideanDistance(ni.points, nj.points, colSize);
                    (*dd)[mutualIndex] = dist;
                    mutualIndex++;

                    if (dist == 0) {
                        // нейроны идентичны
//                            ni.data.skipped = true;
                        // TODO: необходимо удалять идентичные нейроны, иначе отображение sammon не будет построенно
                        // другими словами среди взаимных расстояний не может быть == 0
                        // если нейрон был удален то уже трудно будет построить линии на графике
                        // (точки при этом строятся в не зависимости был ли удален нейрон или нет)
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
        double doOneIteration() {
            for (size_t j = 0; j < nLen; ++j) {
                double e1x = 0, e1y = 0, e2x = 0, e2y = 0;
                for (size_t k = 0; k < nLen; ++k) {
                    if (j != k) {
                        double xd = (*mapPoints)[j].x - (*mapPoints)[k].x;
                        double yd = (*mapPoints)[j].y - (*mapPoints)[k].y;
                        // TODO: непонятно почему к double приводится только
                        // первый слагаемый - если поставить (double)
                        // ко второму то результат будет другим
                        double dpj = std::sqrt(xd * xd + yd * yd);

                        /* calculate derivatives */
                        double dt;
                        if (k > j) {
                            dt = (*dd)[k * (k - 1) / 2 + j];
                        } else {
                            dt = (*dd)[j * (j - 1) / 2 + k];
                        }
                        double dq = dt - dpj;
                        double dr = dt * dpj;
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
            double xx = 0, yy = 0;
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
            double e = 0, tot = 0;
            size_t mutualIndex = 0;
            for (size_t j = 1; j < nLen; ++j) {
                for (size_t k = 0; k < j; ++k) {
                    double dist = (*dd)[mutualIndex];
                    tot += dist;
                    double xd = (*mapPoints)[j].x - (*mapPoints)[k].x;
                    double yd = (*mapPoints)[j].y - (*mapPoints)[k].y;
                    // TODO: непонятно почему к double приводится только
                    // первый слагаемый - если поставить (double)
                    // ко второму то результат будет другим
                    double ee = dist - std::sqrt(xd * xd + yd * yd);
                    e += (ee * ee / dist);
                    mutualIndex++;
                }
            }

            e /= tot;
//            std::cout << "Mapping error " << e << std::endl;
            return e;
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
        utils::CArrayList<double> *dd;

    };
}


#endif
