#ifndef SRC_KOHONEN_UMAT_RECTUMAT_H
#define SRC_KOHONEN_UMAT_RECTUMAT_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "UMat.h"
#include "utils/R3DMatrix.h"

namespace kohonen {
    namespace umat {

        template<typename C, typename T>
        class RectUMat : public UMat<typename C, typename T> {
        public:
            RectUMat() {
            }

            void createUMatrix(long xdim, long ydim, long dim, utils::R3DMatrix<bool, bool, T>& mvalue) {
                for (size_t j = 0; j < ydim; ++j) {
                    for (size_t i = 0; i < xdim; ++i) {
                        double dx = 0, dy = 0, dz1 = 0, dz2 = 0;
                        long count = 0, bx = 0, by = 0, bz = 0;


                        for (long k = 0; k < dim; k++) {

                            if (i < (xdim - 1)) {
                                T temp = (mvalue[i][j][k] - mvalue[i + 1][j][k]);
                                dx += temp * temp;
                                bx = 1;
                            }
                            if (j < (ydim - 1)) {
                                T temp = (mvalue[i][j][k] - mvalue[i][j + 1][k]);
                                dy += temp * temp;
                                by = 1;
                            }

                            if (j < (ydim - 1) && i < (xdim - 1)) {
                                T temp = (mvalue[i][j][k] - mvalue[i + 1][j + 1][k]);
                                dz1 += temp * temp;
                                temp = (mvalue[i][j + 1][k] - mvalue[i + 1][j][k]);
                                dz2 += temp * temp;
                                bz = 1;
                            }
                        }
                        dz = (sqrt(dz1) / sqrt((double) 2.0) + sqrt(dz2) / sqrt((double) 2.0)) / 2;

                        if (bx)
                            umat->uvalue[2 * i + 1][2 * j] = sqrt(dx);
                        if (by)
                            umat->uvalue[2 * i][2 * j + 1] = sqrt(dy);
                        if (bz)
                            umat->uvalue[2 * i + 1][2 * j + 1] = dz;
                    }
                }

            }

        };
    }
}


#endif
