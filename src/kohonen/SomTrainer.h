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
#include "kohonen/neighadap/NeighborAdaptation.h"

namespace kohonen {

    template<typename In, typename Out>
    class SomTrainer {
    public:
        SomTrainer(file::stream::StreamReader<In> *streamReader,
                   alphafunc::AlphaFunction<Out> *_alphaFunction,
                   winner::WinnerSearch<In, Out> *_winnerSearcher,
                   neighadap::NeighborAdaptation<In, Out> *_neighborAdaptation,
                   Out _alpha, Out _radius, size_t _xdim, size_t _ydim)
                : dataReader(streamReader),
                  alphaFunction(_alphaFunction),
                  winnerSearcher(_winnerSearcher),
                  neighborAdaptation(_neighborAdaptation),
                  alpha(_alpha), radius(_radius), xdim(_xdim), ydim(_ydim) {
        }

        /**
         * Обучение
         */
        bool training(utils::SMatrix<Out> *initializedSom,
                      size_t teachSize, size_t colSize) {
            for (size_t le = 0; le < teachSize; ++le) {

                In inRow[colSize];
                bool hasInRow = dataReader->readNext(inRow, colSize);
                if (!hasInRow) {
                    // значит достигли конца данных, начинаем читать с начала
                    // установить поток на начало
                    dataReader->rewindReader();

                    // читаем первую стоку данных
                    hasInRow = dataReader->readNext(inRow, colSize);
                    if (!hasInRow) {
                        /* TODO: couldn't rewind data: NO input data */
                        danger_text("couldn't rewind data");
                        fprintf(stderr, "SomTraining: couldn't rewind data"
                                " (%ld/%ld iterations done)\n", le, teachSize);
                        return false;
                    }
                }


                /* Radius decreases linearly to one */
                Out trad = 1 + (radius - 1) * ((Out) (teachSize - le)) /
                                 (Out) teachSize;
                Out talp = alphaFunction->calcAlpha(le, teachSize, alpha);

//                std::cout<< le << " trad: " << trad << ", talp: " << talp << std::endl;

                /*
                 * If the sample is weighted, we
                 * modify the training rate so that we achieve the same effect
                 * as repeating the sample 'weight' times
                 */
//                if ((weight > 0.0) && (use_weights(-1))) {
//                    talp = 1.0 - (float) pow((double) (1.0 - talp), (double) weight);
//                }

                winner::WinnerInfo<Out> wInfo;
                wInfo.diff = -1;
                bool ok = winnerSearcher->search(initializedSom, inRow, &wInfo);
                // TODO ok == false только если весь вектор пуст
                if (ok) {
//                    long winnerIndex;
//                    if (wInfo.codeIndexes->size() == 0) {
//                        // победитель не найден в случае если все значения
//                        // в inRow пусты
//                        winnerIndex = -1;
//                    } else {
//
//                    }

                    long winnerIndex = wInfo.codeIndexes->getArray()[0];

                    // координаты нейрона в решетке
                    long bxind = winnerIndex % xdim;
                    long byind = winnerIndex / xdim;
//                    std::cout<< le << " winnerIndex: " << winnerIndex << " diff: " << wInfo.diff << std::endl;

                    /* Adapt the units */
                    neighborAdaptation->adaptation(initializedSom, inRow,
                                                   bxind, byind, trad, talp);
                } else {
                    // skip inRow for calculation
                    fprintf(stderr, "ignoring sample %d\n", le);
                }

            }
        }

    private:
        // поток входных данных
        file::stream::StreamReader<In> *dataReader;

        // альфа функция
        alphafunc::AlphaFunction<Out> *alphaFunction;

        // параметры обучения:

        Out alpha;
        Out radius;
        size_t xdim;
        size_t ydim;

        winner::WinnerSearch<In, Out> *winnerSearcher;
        neighadap::NeighborAdaptation<In, Out> *neighborAdaptation;
    };
}


#endif
