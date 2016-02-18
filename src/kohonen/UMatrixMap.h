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

        UMatrixMap(long _xdim, long _ydim) :
                xdim(_xdim), ydim(_ydim),
                mvalue(new utils::R3DMatrix<Out>(xdim, ydim)) {
        }

        ~UMatrixMap() {
            delete mvalue;
        }

//        struct Point {
//            Out x;
//            Out y;
//        };

        void initializeMap(OutCodes *trainedSom) {
            size_t neuronIndex = 0;
            for (size_t j=0; j < ydim; ++j) {
                for (size i = 0; i < xdim; ++i) {
                    // write element to [i][j]
                    mvalue->writePoints(i, j, trainedSom->getRowPoints(neuronIndex), trainedSom->getColSize());
                    ++neuronIndex;
                }
            }
        }

//        void doOneIteration() {
//        }
//
//        /**
//         * Построение проекций Саммона
//         */
//        void buildMap() {
//        }
//
//        utils::CArrayList<Point> *getMapPoints() const {
//            return mapPoints;
//        }

    private:

        utils::R3DMatrix<Out>* mvalue;

        long xdim;
        long ydim;

//        utils::CArrayList<Point> *mapPoints;
//        utils::CArrayList<Point> *correctionPoints;
//        utils::CArrayList<Out> *dd;

    };
}


#endif
