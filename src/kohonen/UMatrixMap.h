/**
 * Построение U-матрицы для SOM
 */

#ifndef SRC_KOHONEN_UMATRIXMAP_H
#define SRC_KOHONEN_UMATRIXMAP_H

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
#include "utils/R3DMatrix.h"
#include "utils/CArrayList.h"
#include "utils/ArrayUtils.h"
#include "file/stream/StreamReader.h"
#include "kohonen/alphafunc/AlphaFunction.h"
#include "kohonen/neighadap/NeighborAdaptation.h"
#include "models/DataSample.h"
#include "utils/console_colors.h"

namespace kohonen {

    template<typename Out>
    class UMatrixMap {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;
        typedef typename utils::RMatrix<models::NeuronInfo, Out>::Row Neuron;
        typedef utils::ArrayUtils<Out, Out, Out> ArrayUtils;

        UMatrixMap(long _nLen) :
                nLen(_nLen),
                mvalue(new utils::R3DMatrix<Out>(nLen, 1, nLen)),
                correctionPoints(new utils::CArrayList<Point>(nLen)),
                dd(new utils::CArrayList<Out>(nLen * (nLen - 1) / 2)) {
        }

        ~UMatrixMap() {
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

        void doOneIteration() {
        }

        /**
         * Построение проекций Саммона
         */
        void buildMap() {
        }

        utils::CArrayList<Point> *getMapPoints() const {
            return mapPoints;
        }

    private:

        utils::R3DMatrix<Out>* mvalue;

        long nLen;
        utils::CArrayList<Point> *mapPoints;
        utils::CArrayList<Point> *correctionPoints;
        utils::CArrayList<Out> *dd;

    };
}


#endif
