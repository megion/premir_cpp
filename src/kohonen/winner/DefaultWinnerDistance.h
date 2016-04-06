#ifndef SRC_KOHONEN_WINNER_DEFAULTWINNERDISTANCE_H
#define SRC_KOHONEN_WINNER_DEFAULTWINNERDISTANCE_H

#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>

#include "utils/console_colors.h"
#include "WinnerDistance.h"

namespace kohonen {
    namespace winner {

        class DefaultWinnerDistance : public WinnerDistance {
        public:
            DefaultWinnerDistance() : WinnerDistance() {
            }

            bool distance(double *somNeuronCodes, models::DataSample *samples, size_t dim, double &difference,
                          const double &maxDifference) const {
                size_t masked = 0;
                difference = 0;
                /* Compute the distance between codebook and input entry */
                for (size_t i = 0; i < dim; ++i) {
                    if (samples[i].skipped) {
                        masked++;
                        /* ignore vector components that skipped */
                        continue;
                    }

                    double diff = somNeuronCodes[i] - samples[i].value;
                    difference += diff * diff;
                    if (difference > maxDifference) {
                        break;
                    }
                }

                if (masked == dim) {
                    /* TODO: can't calculate winner, empty data vector */
//                    danger_text("can't calculate winner, empty data vector");
                    return false;
                }

                return true;
            }

        };
    }
}


#endif
