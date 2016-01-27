/**
 * Функции поиска победителей
 */

#ifndef SRC_KOHONEN_WINNERFUNCTIONS_H
#define SRC_KOHONEN_WINNERFUNCTIONS_H

#include <stdio.h>

#include "utils/SMatrix.h"

namespace kohonen {

    template<typename InStreamReader, typename In, typename Out>
    class WinnerFunctions {

    public:

        /**
         * finds the winning entry (1 nearest neighbour) in
         * codebook using euclidean distance. Information about the winning
         * entry is saved in the winner_info structure. Return 1 (the number
         * of neighbours) when successful and 0 when winner could not be found
         * (for example, all components of data vector have been masked off)
         */
        static int findWinnerEuclidean(utils::SMatrix<Out> *somCodes,
                                       InStreamReader *dataReader,
                                       struct winner_info *win, int knn) {
            struct data_entry *codetmp;
            int dim, i, masked;
            float diffsf, diff, difference;
            eptr p;

            size_t dim = codes->dimension;

            win->index = -1;
            win->winner = NULL;
            win->diff = -1.0;

            /* Go through all code vectors */
            codetmp = rewind_entries(codes, &p);
            diffsf = FLT_MAX;

            while (codetmp != NULL) {
                difference = 0.0;
                masked = 0;

                /* Compute the distance between codebook and input entry */
                for (i = 0; i < dim; i++) {
                    if ((sample->mask != NULL) && (sample->mask[i] != 0)) {
                        masked++;
                        continue; /* ignore vector components that have 1 in mask */
                    }
                    diff = codetmp->points[i] - sample->points[i];
                    difference += diff * diff;
                    if (difference > diffsf) break;
                }

                if (masked == dim)
                    return 0; /* can't calculate winner, empty data vector */

                /* If distance is smaller than previous distances */
                if (difference < diffsf) {
                    win->winner = codetmp;
                    win->index = p.index;
                    win->diff = difference;
                    diffsf = difference;
                }

                codetmp = next_entry(&p);
            }

            if (win->index < 0)
                ifverbose(3);
            fprintf(stderr, "find_winner_euc: can't find winner\n");

            return 1; /* number of neighbours */
        }

    private:
        AlphaFunctions() {
        }

    };
}

#endif
