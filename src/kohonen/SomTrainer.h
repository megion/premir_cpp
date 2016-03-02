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
#include "utils/RMatrix.h"
#include "file/stream/StreamReader.h"
#include "kohonen/alphafunc/AlphaFunction.h"
#include "kohonen/neighadap/NeighborAdaptation.h"
#include "models/models.h"
#include "utils/console_colors.h"

namespace kohonen {

    template<typename InRow, typename InNum, typename Out>
    class SomTrainer {
    public:

        typedef file::stream::StreamReader<InRow, InNum> InReader;
        typedef utils::RMatrix<models::NeuronInfo, Out> OutCodes;

        SomTrainer(alphafunc::AlphaFunction<Out> *_alphaFunction,
                winner::WinnerSearch<InNum, Out> *_winnerSearcher,
                neighadap::NeighborAdaptation<InNum, Out> *_neighborAdaptation,
                Out _alpha, Out _radius, size_t _xdim, size_t _ydim)
                : alphaFunction(_alphaFunction),
                  winnerSearcher(_winnerSearcher),
                  neighborAdaptation(_neighborAdaptation),
                  alpha(_alpha), radius(_radius), xdim(_xdim), ydim(_ydim) {
        }

        struct QuantumError {
            /* сумма расстояний от каждого входного вектора до победителя */
            Out sumWinnerDistance;
            /* число входных данных на которых посчитана ошибка квантования */
            size_t samplesSize;
        };

        /**
         * Обучение указанное teachSize число раз
         */
        bool training(OutCodes *initializedSom, InReader *inDataStreamReader, size_t teachSize) {
            // установить поток на начало
            inDataStreamReader->rewindReader();

            size_t winnerSize = winnerSearcher->getWinnerSize();
            size_t colSize = initializedSom->getColSize();

            for (size_t le = 0; le < teachSize; ++le) {
                models::DataSample<InNum> samples[colSize];
                InRow rowData;
                bool hasInRow = inDataStreamReader->readNext(rowData, samples);
                if (!hasInRow) {
                    // значит достигли конца данных, начинаем читать с начала
                    // установить поток на начало
                    inDataStreamReader->rewindReader();

                    // читаем первую стоку данных
                    hasInRow = inDataStreamReader->readNext(rowData, samples);
                    if (!hasInRow) {
                        /* TODO: couldn't rewind data: NO input data */
                        danger_text("couldn't rewind data");
                        fprintf(stderr, "SomTraining: couldn't rewind data(%ld/%ld iterations done)\n", le, teachSize);
                        return false;
                    }
                }

                winner::WinnerInfo<Out> winners[winnerSize];
                bool ok = trainingBySample(initializedSom, samples, winners, teachSize, le);
                if (!ok) {
                    std::cerr << "skip empty sample " << le << std::endl;
                }
            }
            return true;
        }

        bool trainingBySample(OutCodes *initializedSom,
                              models::DataSample<InNum> *sampleVector,
                              winner::WinnerInfo<Out> *winners,
                              size_t teachSize, size_t index) {
            /* Radius decreases linearly to one */
            Out trad = 1 + (radius - 1) * ((Out) (teachSize - index)) / (Out) teachSize;
            Out talp = alphaFunction->calcAlpha(index, teachSize, alpha);

            bool ok = winnerSearcher->search(initializedSom, sampleVector, winners);
            // TODO ok == false только если весь вектор пуст
            if (ok) {
                long winnerIndex = winners[0].index;

                // координаты нейрона в решетке
                long bxind = winnerIndex % xdim;
                long byind = winnerIndex / xdim;

                /* Adapt the units */
                neighborAdaptation->adaptation(initializedSom, sampleVector, bxind, byind, trad, talp);
                return true;
            } else {
                // skip inRow for calculation
                danger_text("skip empty sample vector");
                return false;
            }
        }

        /**
         * Вычисление ошибки квантования
         */
        QuantumError quantizationError(OutCodes *trainedSom, InReader *inDataStreamReader, size_t rowsLimit = 0) {
            QuantumError qError = {0, 0};

            // установить поток на начало
            inDataStreamReader->rewindReader();

            size_t winnerSize = winnerSearcher->getWinnerSize();
            size_t colSize = trainedSom->getColSize();

            models::DataSample<InNum> samples[colSize];
            InRow rowData;
            size_t rowIndex = 0;
            while (inDataStreamReader->readNext(rowData, samples) && (rowsLimit==0 || (rowIndex<rowsLimit))) {
                winner::WinnerInfo<Out> wInfo[winnerSize];
                bool ok = winnerSearcher->search(trainedSom, samples, wInfo);
                // TODO ok == false только если весь вектор пуст
                if (ok) {
//                    long winnerIndex = wInfo[0].index;
//                    // координаты нейрона в решетке
//                    long bxind = winnerIndex % xdim;
//                    long byind = winnerIndex / xdim;

                    qError.sumWinnerDistance += std::sqrt(wInfo[0].diff);
                    qError.samplesSize++;
                }
                rowIndex++;
            }

            return qError;
        }

    private:
        // альфа функция
        alphafunc::AlphaFunction<Out> *alphaFunction;

        // параметры обучения:
        Out alpha;
        Out radius;
        size_t xdim;
        size_t ydim;

        winner::WinnerSearch<InNum, Out> *winnerSearcher;
        neighadap::NeighborAdaptation<InNum, Out> *neighborAdaptation;
    };
}


#endif
