#ifndef SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H
#define SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H

#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>

#include "utils/SMatrix.h"
#include "WinnerSearch.h"
#include "WinnerInfo.h"
#include "utils/console_colors.h"

namespace kohonen {
    namespace winner {

        /**
         * finds the winning entry (1 nearest neighbour) in
         * codebook using euclidean distance. Information about the winning
         * entry is saved in the winner_info structure. Return 1 (the number
         * of neighbours) when successful and 0 when winner could not be found
         * (for example, all components of data vector have been masked off)
         *
         */
        template<typename In, typename Out>
        class EuclideanWinnerSearch : public WinnerSearch<In, Out> {
        public:
            EuclideanWinnerSearch() {
            }

            bool search(utils::SMatrix<Out> *somCodes, In* inSampleRow,
                        WinnerInfo<Out>* winner) {
                size_t dim = somCodes->getColSize();

                winner->diff = -1.0;
//                size_t winnerIndex = NAN;
                Out maxDifference = std::numeric_limits<Out>::max();

                for (size_t r=0; r<somCodes->getRowSize(); ++r) {
                    size_t masked = 0;
                    Out difference = 0;
                    /* Compute the distance between codebook and input entry */
                    for (size_t i = 0; i < dim; ++i) {
                        if (std::isnan(inSampleRow[i])) {
                            masked++;
                            /* ignore vector components that have 1 in mask */
                            continue;
                        }
                        Out diff = (*somCodes)(r, i) - inSampleRow[i];
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
                        if (winner->codeIndexes->size()==0) {
                            winner->codeIndexes->push(r);
                        } else {
                            (*winner->codeIndexes)[0] = r;
                        }
                        winner->diff = difference;
                        maxDifference = difference;
                    }
                }

                if (winner->codeIndexes->size()==0) {
                    /* TODO: can't find winner */
                    danger_text("EuclideanWinnerSearch: can't find winner");
                }

                return true;
            }

        };
    }
}


#endif
