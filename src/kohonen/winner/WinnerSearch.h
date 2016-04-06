#ifndef SRC_KOHONEN_WINNER_WINNERSEARCH_H
#define SRC_KOHONEN_WINNER_WINNERSEARCH_H

#include <cstdio>

#include "utils/RMatrix.h"
#include "models/models.h"
#include "WinnerDistance.h"

namespace kohonen {
    namespace winner {

        struct WinnerInfo {
            double diff; // вычисленное расстояние
            long index; // индекс нейрона победителя
        };

        class WinnerSearch {
        public:

            WinnerSearch(size_t _winnersSize, WinnerDistance *_winnerDistance) :
                    winnersSize(_winnersSize),
                    winnerDistance(_winnerDistance) {
            }

            virtual bool search(utils::RMatrix<models::NeuronInfo, double> *somCodes, models::DataSample *inSampleRow,
                                WinnerInfo *winners) = 0;

            size_t getWinnerSize() {
                return winnersSize;
            }

        protected:
            size_t winnersSize;

            WinnerDistance *winnerDistance;

        };
    }
}


#endif
