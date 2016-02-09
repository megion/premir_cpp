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
#include "utils/ArrayUtils.h"
#include "file/stream/StreamReader.h"
#include "kohonen/alphafunc/AlphaFunction.h"
#include "kohonen/neighadap/NeighborAdaptation.h"
#include "models/DataSample.h"

namespace kohonen {

    template<typename Out>
    class SammonMap {
    public:

        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;
        typedef utils::RMatrix<models::NeuronInfo, Out>::Row Neuron;
        typedef utils::ArrayUtils<Out, Out, Out> ArrayUtils;

        SammonMap(){
            randomEngine = new RandomGenerator();
        }

        ~SammonMap() {
            delete randomEngine;
        }

        /**
         * Построение проекций Саммона
         */
        bool sammon(OutCodes *trainedSom,
                      size_t iterationLen) {

            setSkippedIdenticalNeurons(trainedSom);

            long neuronsLen = trainedSom->getRowSize();
            Out x[neuronsLen];
            Out y[neuronsLen];
            Out xu[neuronsLen];
            Out yu[neuronsLen];
            Out dd[neuronsLen*(neuronsLen-1)/2];

            for (size_t i = 0; i < neuronsLen; ++i) {
                x[i] = (Out) (randomEngine->generate() % neuronsLen) / neuronsLen;
                y[i] = (Out) (i) / neuronsLen;
            }

            for (size_t le = 0; le < iterationLen; ++le) {

            }
            return true;
        }



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

        RandomGenerator *getRandomGenerator() const {
            return randomEngine;
        }

    private:
        // генератор псевдо случайных чисел
        RandomGenerator *randomEngine;
    };
}


#endif
