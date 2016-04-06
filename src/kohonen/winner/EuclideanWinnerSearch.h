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
        class EuclideanWinnerSearch : public WinnerSearch {
        public:
            EuclideanWinnerSearch(WinnerDistance *_winnerDistance) : WinnerSearch(1, _winnerDistance) {
            }

            bool search(utils::RMatrix<models::NeuronInfo, double> *somCodes, models::DataSample *inSampleRow,
                        WinnerInfo *winners) {
                size_t dim = somCodes->getColSize();

                winners[0].diff = -1;
                winners[0].index = -1;

                double maxDifference = std::numeric_limits<double>::max();

                for (size_t r = 0; r < somCodes->getRowSize(); ++r) {
                    double difference = 0;
                    bool ok = winnerDistance->distance((*somCodes)[r], inSampleRow, dim, difference, maxDifference);
                    if (!ok) {
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

                if (winners[0].index < 0) {
                    /* TODO: can't find winner */
                    danger_text("EuclideanWinnerSearch: can't find winner");
                }

                return true;
            }

        };
    }
}


#endif
