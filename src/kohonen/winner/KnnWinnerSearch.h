#ifndef SRC_KOHONEN_WINNER_KNNWINNERSEARCH_H
#define SRC_KOHONEN_WINNER_KNNWINNERSEARCH_H

#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>

#include "utils/SMatrix.h"
#include "WinnerSearch.h"
#include "utils/console_colors.h"

namespace kohonen {
    namespace winner {

        /**
         * KnnWinnerSearch - finds the winning entrys (k nearest neighbours)
         * in codebook using euclidean distance. Information about the winning
         * entry is saved in the WinnerInfo structures provided by the caller.
         * Return true when successful and 0 when winner could not be found
         * (for example, all components of data vector have been masked off)
         *
         * TODO: KnnWinnerSearch не используется в при обучении в SOM
         * используется она только в LVQ
         */
        template<typename In, typename Out>
        class KnnWinnerSearch : public WinnerSearch<In, Out> {
        public:
            KnnWinnerSearch(size_t _winnersSize) :
                    WinnerSearch<In, Out>(_winnersSize) {
            }

            bool search(utils::RMatrix<models::NeuronInfo, Out> *somCodes,
                        models::DataSample<In> *inSampleRow,
                        WinnerInfo<Out> *winners) {
                size_t knn = WinnerSearch<In, Out>::winnersSize;
                size_t dim = somCodes->getColSize();
                Out maxDifference = std::numeric_limits<Out>::max();

                for (size_t i = 0; i < winnersSize; ++i) {
                    winners[i].index = -1;
                    winners[i].diff = maxDifference;
                }

                for (size_t r = 0; r < somCodes->getRowSize(); ++r) {
                    size_t masked = 0;
                    Out difference = 0;

                    /* Compute the distance between codebook and input entry */
                    for (size_t i = 0; i < dim; ++i) {
                        if (inSampleRow[i].skipped) {
                            masked++;
                            /* ignore vector components that skipped */
                            continue;
                        }

                        Out diff = (*somCodes)(r, i) - inSampleRow[i].value;
                        difference += diff * diff;
                        if (difference > winners[knn - 1].diff) {
                            break;
                        }
                    }

                    if (masked == dim) {
                        /* TODO: can't calculate winner, empty data vector */
                        danger_text(
                                "can't calculate winner, empty data vector");
                        return false;
                    }

                    /* If distance is smaller than previous distances */
                    // найти текущий максимальный номер победителя у которого
                    // diff <= difference
                    size_t lastWinnerNumber = 0;
                    for (size_t i = 0;
                         (i < knn) && (difference > winners[i].diff); ++i) {
                        lastWinnerNumber++;
                    }

                    if (lastWinnerNumber < knn) {
                        for (long j = knn - 1; j > lastWinnerNumber; j--) {
                            winners[j].diff = winners[j - 1].diff;
                            winners[j].index = winners[j - 1].index;
                        }

                        winners[lastWinnerNumber].diff = difference;
                        winners[lastWinnerNumber].index = r;
                    }
                }

                if (winners[0].index < 0) {
                    /* TODO: can't find winner */
                    danger_text("KnnWinnerSearch: can't find winner");
                }

                return true;
            }

        };
    }
}


#endif
