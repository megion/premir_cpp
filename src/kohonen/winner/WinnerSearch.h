#ifndef SRC_KOHONEN_WINNER_WINNERSEARCH_H
#define SRC_KOHONEN_WINNER_WINNERSEARCH_H

#include <cstdio>

#include "utils/RMatrix.h"
#include "models/models.h"

namespace kohonen {
    namespace winner {

        struct WinnerInfo {
            double diff; // вычисленное расстояние
            long index; // индекс нейрона победителя
        };

        class WinnerSearch {
        public:

            WinnerSearch(size_t _winnersSize) : winnersSize(_winnersSize) {
            }

            virtual bool search(utils::RMatrix<models::NeuronInfo, double> *somCodes, models::DataSample *inSampleRow,
                                WinnerInfo *winners) = 0;

            size_t getWinnerSize() {
                return winnersSize;
            }

        protected:
            size_t winnersSize;

        };
    }
}


#endif
