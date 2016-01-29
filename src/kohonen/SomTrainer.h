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
//#include <kohonen/winner/WinnerSearch.h>

#include "utils/SMatrix.h"
#include "kohonen/winner/WinnerSearch.h"

namespace kohonen {

    template<typename InStreamReader, typename In, typename Out>
    class SomTrainer {
    public:
        SomTrainer(InStreamReader *_inStream,
                   winner::WinnerSearch<Out> *_winnerSearcher,
                   Out _alpha, Out _radius)
                : dataReader(_inStream), winnerSearcher(_winnerSearcher),
                  alpha(_alpha), radius(_radius) {
        }

//        struct WinnerInfo {
//            size_t index;
//            xcb_point_t labelPoint;
//            double labelValue;
//            bool hideLabel;
//        };

        /**
         * Обучение
         */
        utils::SMatrix<Out> *training(utils::SMatrix<Out> *initializedSom,
                                      size_t teachSize) {
            for (size_t le = 0; le < teachSize; ++le) {

                /* Radius decreases linearly to one */
//                Out trad = 1.0 + (radius - 1.0) * (Out) (teachSize - le) /
//                                 (Out) teachSize;
//                Out talp = alphaFunc(le, teachSize, alpha);

                /*
                 * If the sample is weighted, we
                 * modify the training rate so that we achieve the same effect
                 * as repeating the sample 'weight' times
                 */
//                if ((weight > 0.0) && (use_weights(-1))) {
//                    talp = 1.0 - (float) pow((double) (1.0 - talp), (double) weight);
//                }

                winnerSearcher->search(initializedSom);

            }
        }

    private:
        // поток входных данных
        InStreamReader *dataReader;

        // параметры обучения:

        Out alpha;
        Out radius;

        // указатель на альфа функцию
        Out (*alphaFunc)(size_t, size_t, Out alpha);

        winner::WinnerSearch<Out> *winnerSearcher;
    };
}


#endif
