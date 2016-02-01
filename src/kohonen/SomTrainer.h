/**
 * Обучение карты SOM
 */

#ifndef SRC_KOHONEN_SOMTRAINER_H
#define SRC_KOHONEN_SOMTRAINER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/SMatrix.h"
#include "kohonen/winner/WinnerSearch.h"
#include "file/stream/StreamReader.h"
#include "kohonen/alphafunc/AlphaFunction.h"

namespace kohonen {

    template<typename In, typename Out>
    class SomTrainer {
    public:
        SomTrainer(file::stream::StreamReader<In> *streamReader,
                   alphafunc::AlphaFunction<Out>* _alphaFunction,
                   winner::WinnerSearch<In, Out> *_winnerSearcher,
                   Out _alpha, Out _radius)
                : dataReader(streamReader), alphaFunction(_alphaFunction),
                  winnerSearcher(_winnerSearcher), alpha(_alpha),
                  radius(_radius) {
        }

        /**
         * Обучение
         */
        utils::SMatrix<Out> *training(utils::SMatrix<Out> *initializedSom,
                                      size_t teachSize) {
            for (size_t le = 0; le < teachSize; ++le) {

                /* Radius decreases linearly to one */
                Out trad = 1.0 + (radius - 1.0) * (Out) (teachSize - le) /
                                 (Out) teachSize;
                Out talp = alphaFunction->calcAlpha(le, teachSize, alpha);

                /*
                 * If the sample is weighted, we
                 * modify the training rate so that we achieve the same effect
                 * as repeating the sample 'weight' times
                 */
//                if ((weight > 0.0) && (use_weights(-1))) {
//                    talp = 1.0 - (float) pow((double) (1.0 - talp), (double) weight);
//                }

//                winnerSearcher->search(initializedSom);

            }
        }

    private:
        // поток входных данных
        file::stream::StreamReader<In> *dataReader;

        // альфа функция
        alphafunc::AlphaFunction<Out>* alphaFunction;

        // параметры обучения:

        Out alpha;
        Out radius;

        winner::WinnerSearch<In, Out> *winnerSearcher;
    };
}


#endif
