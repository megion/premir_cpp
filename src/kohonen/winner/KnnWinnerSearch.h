#ifndef SRC_KOHONEN_WINNER_KNNWINNERSEARCH_H
#define SRC_KOHONEN_WINNER_KNNWINNERSEARCH_H

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
        class KnnWinnerSearch : public WinnerSearch<In, Out> {
        public:
            KnnWinnerSearch() {
            }

            bool search(utils::SMatrix<Out> *somCodes, In* inSampleRow,
                        WinnerInfo<Out>* winner) {
                std::cout << "call KnnWinnerSearch" << std::endl;

                size_t dim = somCodes->getColSize();

                winner->diff = -1.0;
                size_t winnerIndex = NAN;
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
                        Out diff = somCodes(r, i) - inSampleRow[i];
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
                            winner->codeIndexes[0] = r;
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

            /* find_winner_knn - finds the winning entrys (k nearest neighbours)
   in codebook using euclidean distance. Information about the winning
   entry is saved in the winner_info structures provided by the
   caller. Return k (the number of neighbours) when successful and 0
   when winner could not be found (for example, all components of data
   vector have been masked off) */

//            int find_winner_knn(struct entries *codes, struct data_entry *sample,
//                                struct winner_info *win, int knn)
//            {
//                struct data_entry *codetmp;
//                int dim, i, j, masked;
//                float difference, diff;
//                eptr p;
//
//                if (knn == 1) /* might be a little faster */
//                    return find_winner_euc(codes, sample, win, 1);
//
//                dim = codes->dimension;
//
//                for (i = 0; i < knn; i++)
//                {
//                    win[i].index = -1;
//                    win[i].winner = NULL;
//                    win[i].diff = FLT_MAX;
//                }
//                /* Go through all code vectors */
//
//                codetmp = rewind_entries(codes, &p);
//
//                while (codetmp != NULL) {
//                    difference = 0.0;
//
//                    masked = 0;
//                    /* Compute the distance between codebook and input entry */
//                    for (i = 0; i < dim; i++)
//                    {
//                        /* pitaisiko ottaa huomioon myos codebookissa olevat?? */
//                        if ((sample->mask != NULL) && (sample->mask[i] != 0))
//                        {
//                            masked++;
//                            continue; /* ignore vector components that have 1 in mask */
//                        }
//                        diff = codetmp->points[i] - sample->points[i];
//                        difference += diff * diff;
//                        if (difference > win[knn-1].diff) break;
//                    }
//
//                    if (masked == dim)
//                        return 0;
//
//                    /* If distance is smaller than previous distances */
//                    for (i = 0; (i < knn) && (difference > win[i].diff); i++);
//
//                    if (i < knn)
//                    {
//                        for (j = knn - 1; j > i; j--)
//                        {
//                            win[j].diff = win[j - 1].diff;
//                            win[j].index = win[j - 1].index;
//                            win[j].winner = win[j - 1].winner;
//                        }
//
//                        win[i].diff = difference;
//                        win[i].index = p.index;
//                        win[i].winner = codetmp;
//                    }
//
//                    codetmp = next_entry(&p);
//                }
//
//                if (win->index < 0)
//                    ifverbose(3)
//                fprintf(stderr, "find_winner_knn: can't find winner\n");
//
//                return knn; /* number of neighbours */
//            }

        };
    }
}


#endif
