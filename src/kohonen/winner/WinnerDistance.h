#ifndef SRC_KOHONEN_WINNER_WINNERDISTANCE_H
#define SRC_KOHONEN_WINNER_WINNERDISTANCE_H

#include <cstdio>
#include <limits>


#include "utils/RMatrix.h"
#include "models/models.h"

namespace kohonen {
    namespace winner {

        /**
         * Create own winner distance for customize winner distance
         */
        class WinnerDistance {
        public:

            WinnerDistance() {
            }

            virtual bool distance(double *somNeuronCodes, models::DataSample *samples, size_t dim,
                                  double &difference, const double &maxDifference) const = 0;

        };
    }
}


#endif
