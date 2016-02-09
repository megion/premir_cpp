/**
 * Построение проекций Саммона
 */

#ifndef SRC_KOHONEN_SAMMONMAP_H
#define SRC_KOHONEN_SAMMONMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/SMatrix.h"
#include "utils/RMatrix.h"
#include "file/stream/StreamReader.h"
#include "kohonen/alphafunc/AlphaFunction.h"
#include "kohonen/neighadap/NeighborAdaptation.h"
#include "models/DataSample.h"

namespace kohonen {

    template<typename In, typename Out>
    class SammonMap {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;

        SammonMap(){
        }

        /**
         * Построение проекций Саммона
         */
        bool training(OutCodes *initializedSom,
                      size_t iterationLen) {


            for (size_t le = 0; le < iterationLen; ++le) {

            }
            return true;
        }

        bool sammonIterationSample(OutCodes *trainedSom,
                              size_t iterationLen,
                              size_t index) {
        }

        /**
         * Для идентичных нейронов (евклидовое расстояние между котороми
         * = 0) установить флаг skipped = true.
         */
        void setSkippedIdenticalNeurons(OutCodes *trainedSom) {

        }

    private:

    };
}


#endif
