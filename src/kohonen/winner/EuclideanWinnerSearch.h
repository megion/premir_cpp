#ifndef SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H
#define SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H

#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>

#include "utils/SMatrix.h"
#include "utils/console_colors.h"
#include "WinnerSearch.h"

namespace kohonen {
    namespace winner {

        /**
         * Finds the winning entry (1 nearest neighbour) in
         * codebook using euclidean distance. Information about the winning
         * entry is saved in the WinnerInfo structure. Return true when
         * successful and 0 when winner could not be found
         * (for example, all components of data vector have been masked off)
         */
        template<typename In, typename Out>
        class EuclideanWinnerSearch : public WinnerSearch<In, Out> {
        public:
            EuclideanWinnerSearch() : WinnerSearch<In, Out>(1) {
            }

            bool search(utils::SMatrix<Out> *somCodes,
                        models::DataSample<In>* inSampleRow,
                        WinnerInfo<Out>* winners) {
                size_t dim = somCodes->getColSize();

                winners[0].diff = -1;
                winners[0].index = -1;

                Out maxDifference = std::numeric_limits<Out>::max();

                for (size_t r=0; r<somCodes->getRowSize(); ++r) {
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
                        if (difference > maxDifference) {
                            break;
                        }
                    }

                    if (masked == dim) {
                        /* TODO: can't calculate winner, empty data vector */
                        danger_text("can't calculate winner, empty data vector");
                        return false;
                    }

                    /* If distance is smaller than previous distances */
                    if (difference < maxDifference) {
                        winners[0].index = r;
                        winners[0].diff = difference;
                        maxDifference = difference;
                    }
                }

                if (winners[0].index<0) {
                    /* TODO: can't find winner */
                    danger_text("EuclideanWinnerSearch: can't find winner");
                }

                return true;
            }

        };
    }
}


#endif
