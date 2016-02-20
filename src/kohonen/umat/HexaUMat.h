#ifndef SRC_KOHONEN_UMAT_HEXAUMAT_H
#define SRC_KOHONEN_UMAT_HEXAUMAT_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "UMat.h"
#include "utils/R3DMatrix.h"

namespace kohonen {
    namespace umat {

        template<typename Out>
        class HexaUMat : public UMat<Out> {
        public:

            typedef utils::RMatrix<models::NeuronInfo, Out> SomCodes;
            typedef utils::RMatrix<models::NeuronInfo, Out> UMatCodes;
            typedef utils::R3DMatrix<bool, models::NeuronInfo, Out> SOMMatrix;

            HexaUMat(size_t _xdim, size_t _ydim, size_t _dim) :
                    UMat<Out>(_xdim, _ydim, _dim) {
            }

            void buildUMatrix() {
                SOMMatrix &mvalue = (*UMat<Out>::somMatrix);
                UMatCodes &uvalue = (*UMat<Out>::uMatrix);

                size_t xdim = UMat<Out>::xdim;
                size_t ydim = UMat<Out>::ydim;
                size_t dim = UMat<Out>::dim;
                size_t uxdim = UMat<Out>::uxdim;
                size_t uydim = UMat<Out>::uydim;

                for (size_t j = 0; j < ydim; ++j) {
                    for (size_t i = 0; i < xdim; ++i) {
                        Out dx = 0, dy = 0, dz = 0;
                        long count = 0, bx = 0, by = 0, bz = 0;

                        if (i < (xdim - 1)) {
                            for (size_t k = 0; k < dim; k++) {
                                Out temp = (mvalue[i][j][k] - mvalue[i + 1][j][k]);
                                dx += temp * temp;
                                bx = 1;
                            }
                        }

                        if (j < (ydim - 1)) {
                            if (j % 2) {
                                for (size_t k = 0; k < dim; k++) {
                                    Out temp = (mvalue[i][j][k] - mvalue[i][j + 1][k]);
                                    dy += temp * temp;
                                    by = 1;
                                }
                            }
                            else {
                                if (i > 0) {
                                    for (size_t k = 0; k < dim; k++) {
                                        Out temp = (mvalue[i][j][k] - mvalue[i - 1][j + 1][k]);
                                        dy += temp * temp;
                                        by = 1;
                                    }
                                }
                            }

                        }

                        if (j < (ydim - 1)) {
                            if (!(j % 2)) {
                                for (size_t k = 0; k < dim; k++) {
                                    Out temp = (mvalue[i][j][k] - mvalue[i][j + 1][k]);
                                    dz += temp * temp;
                                }
                                bz = 1;
                            }
                            else {

                                if (i < (xdim - 1)) {
                                    for (size_t k = 0; k < dim; k++) {
                                        Out temp = (mvalue[i][j][k] - mvalue[i + 1][j + 1][k]);
                                        dz += temp * temp;
                                    }
                                    bz = 1;
                                }
                            }
                        }

                        if (bx) {
                            uvalue[2 * i + 1][2 * j] = std::sqrt(dx);
                        }

                        if (by) {
                            if (j % 2) {
                                uvalue[2 * i][2 * j + 1] = std::sqrt(dy);
                            } else {
                                uvalue[2 * i - 1][2 * j + 1] = std::sqrt(dy);
                            }
                        }

                        if (bz) {
                            if (j % 2) {
                                uvalue[2 * i + 1][2 * j + 1] = sqrt(dz);
                            } else {
                                uvalue[2 * i][2 * j + 1] = sqrt(dz);
                            }
                        }

                    }
                }

                /* medians of the 6-neighborhood */
                Out medtable[6];
                sort::HeapSort<Out> hsort;
                for (size_t j = 0; j < uydim; j += 2) {
                    for (size_t i = 0; i < uxdim; i += 2) {
                        if (i > 0 && j > 0 && i < uxdim - 1 && j < (uydim - 1)) {
                            /* in the middle of the map */
                            medtable[0] = uvalue[i - 1][j];
                            medtable[1] = uvalue[i + 1][j];
                            if (!(j % 4)) {
                                medtable[2] = uvalue[i - 1][j - 1];
                                medtable[3] = uvalue[i][j - 1];
                                medtable[4] = uvalue[i - 1][j + 1];
                                medtable[5] = uvalue[i][j + 1];
                            } else {
                                medtable[2] = uvalue[i][j - 1];
                                medtable[3] = uvalue[i + 1][j - 1];
                                medtable[4] = uvalue[i][j + 1];
                                medtable[5] = uvalue[i + 1][j + 1];
                            }
                            hsort.sort(medtable, 6);
//                            qsort((void *) medtable, 6, sizeof(*medtable), UMat<Out>::compar);
                            /* Actually mean of two median values */
                            uvalue[i][j] = (medtable[2] + medtable[3]) / 2.0;
                        } else if (j == 0 && i > 0 && i < (uxdim - 1)) {
                            /* in the upper edge */
                            medtable[0] = uvalue[i - 1][j];
                            medtable[1] = uvalue[i + 1][j];
                            medtable[2] = uvalue[i][j + 1];
                            medtable[3] = uvalue[i - 1][j + 1];
                            hsort.sort(medtable, 4);
//                            qsort((void *) medtable, 4, sizeof(*medtable), UMat<Out>::compar);
                            /* Actually mean of two median values */
                            uvalue[i][j] = (medtable[1] + medtable[2]) / 2.0;
                        } else if (j == uydim - 1 && i > 0 && i < uxdim - 1) {
                            /* in the lower edge */
                            medtable[0] = uvalue[i - 1][j];
                            medtable[1] = uvalue[i + 1][j];
                            if (!(j % 4)) {
                                medtable[2] = uvalue[i - 1][j - 1];
                                medtable[3] = uvalue[i][j - 1];
                            } else {
                                medtable[2] = uvalue[i][j - 1];
                                medtable[3] = uvalue[i + 1][j - 1];
                            }
                            hsort.sort(medtable, 4);
//                            qsort((void *) medtable, 4, sizeof(*medtable), UMat<Out>::compar);
                            /* Actually mean of two median values */
                            uvalue[i][j] = (medtable[1] + medtable[2]) / 2.0;
                        } else if (i == 0 && j > 0 && j < (uydim - 1)) {
                            /* in the left edge */
                            medtable[0] = uvalue[i + 1][j];
                            if (!(j % 4)) {
                                medtable[1] = uvalue[i][j - 1];
                                medtable[2] = uvalue[i][j + 1];
                                hsort.sort(medtable, 3);
//                                qsort((void *) medtable, 3, sizeof(*medtable), UMat<Out>::compar);
                                uvalue[i][j] = medtable[1];
                            } else {
                                medtable[1] = uvalue[i][j - 1];
                                medtable[2] = uvalue[i + 1][j - 1];
                                medtable[3] = uvalue[i][j + 1];
                                medtable[4] = uvalue[i + 1][j + 1];
                                hsort.sort(medtable, 5);
//                                qsort((void *) medtable, 5, sizeof(*medtable), UMat<Out>::compar);
                                uvalue[i][j] = medtable[2];
                            }
                        } else if (i == (uxdim - 1) && j > 0 && j < (uydim - 1)) {
                            /* in the right edge */
                            medtable[0] = uvalue[i - 1][j];
                            if (j % 4) {
                                medtable[1] = uvalue[i][j - 1];
                                medtable[2] = uvalue[i][j + 1];
                                hsort.sort(medtable, 3);
//                                qsort((void *) medtable, 3, sizeof(*medtable), UMat<Out>::compar);
                                uvalue[i][j] = medtable[1];
                            } else {
                                medtable[1] = uvalue[i][j - 1];
                                medtable[2] = uvalue[i - 1][j - 1];
                                medtable[3] = uvalue[i][j + 1];
                                medtable[4] = uvalue[i - 1][j + 1];
                                hsort.sort(medtable, 5);
//                                qsort((void *) medtable, 5, sizeof(*medtable), UMat<Out>::compar);
                                uvalue[i][j] = medtable[2];
                            }
                        } else if (i == 0 && j == 0)
                            /* the upper left-hand corner */
                            uvalue[i][j] = (uvalue[i + 1][j] + uvalue[i][j + 1]) / 2.0;
                        else if (i == (uxdim - 1) && j == 0) {
                            /* the upper right-hand corner */
                            medtable[0] = uvalue[i - 1][j];
                            medtable[1] = uvalue[i - 1][j + 1];
                            medtable[2] = uvalue[i][j + 1];
                            hsort.sort(medtable, 3);
//                            qsort((void *) medtable, 3, sizeof(*medtable), UMat<Out>::compar);
                            uvalue[i][j] = medtable[1];
                        } else if (i == 0 && j == (uydim - 1)) {
                            /* the lower left-hand corner */
                            if (!(j % 4)) {
                                uvalue[i][j] = (uvalue[i + 1][j] + uvalue[i][j - 1]) / 2.0;
                            } else {
                                medtable[0] = uvalue[i + 1][j];
                                medtable[1] = uvalue[i][j - 1];
                                medtable[2] = uvalue[i + 1][j - 1];
                                hsort.sort(medtable, 3);
//                                qsort((void *) medtable, 3, sizeof(*medtable), UMat<Out>::compar);
                                uvalue[i][j] = medtable[1];
                            }
                        } else if (i == (uxdim - 1) && j == (uydim - 1)) {
                            /* the lower right-hand corner */
                            if (j % 4) {
                                uvalue[i][j] = (uvalue[i - 1][j] + uvalue[i][j - 1]) / 2.0;
                            } else {
                                medtable[0] = uvalue[i - 1][j];
                                medtable[1] = uvalue[i][j - 1];
                                medtable[2] = uvalue[i - 1][j - 1];
                                hsort.sort(medtable, 3);
//                                qsort((void *) medtable, 3, sizeof(*medtable), UMat<Out>::compar);
                                uvalue[i][j] = medtable[1];
                            }
                        }

                    }
                }

                UMat<Out>::scaleUMatrix();
            }

            void averageUMatrix() {
                UMatCodes &uvalue = (*UMat<Out>::uMatrix);

                size_t xdim = UMat<Out>::xdim;
                size_t ydim = UMat<Out>::ydim;
                size_t dim = UMat<Out>::dim;
                size_t uxdim = UMat<Out>::uxdim;
                size_t uydim = UMat<Out>::uydim;

                UMatCodes umat2(uxdim, uydim); // temp

                for (size_t j = 1; j < uydim - 1; j++) {
                    for (size_t i = 1; i < uxdim - 1; i++) {
                        /* Non-borders */
                        if ((j % 4) == 1) {
                            uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i + 1][j - 1] + uvalue[i - 1][j] +
                                             uvalue[i][j] + uvalue[i + 1][j] + uvalue[i - 1][j + 1] +
                                             uvalue[i][j + 1]) / ((float) 7.0));
                        } else if ((j % 4) == 2) {
                            uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i + 1][j - 1] + uvalue[i - 1][j] +
                                             uvalue[i][j] + uvalue[i + 1][j] + uvalue[i][j + 1] +
                                             uvalue[i + 1][j + 1]) / ((float) 7.0));
                        } else if ((j % 4) == 3) {
                            uvalue[i][j] = ((uvalue[i - 1][j - 1] + uvalue[i][j - 1] + uvalue[i - 1][j] +
                                             uvalue[i][j] + uvalue[i + 1][j] + uvalue[i][j + 1] +
                                             uvalue[i + 1][j + 1]) / ((float) 7.0));
                        } else if ((j % 4) == 0) {
                            uvalue[i][j] = ((uvalue[i - 1][j - 1] + uvalue[i][j - 1] + uvalue[i - 1][j] +
                                             uvalue[i][j] + uvalue[i + 1][j] + uvalue[i - 1][j + 1] +
                                             uvalue[i][j + 1]) / ((float) 7.0));
                        }
                    }
                }
                /* north border */
                size_t j = 0;
                for (size_t i = 1; i < uxdim - 1; i++) {
                    uvalue[i][j] = ((uvalue[i - 1][j] + uvalue[i][j] + uvalue[i + 1][j] + uvalue[i - 1][j + 1] +
                                     uvalue[i][j + 1]) / ((float) 5.0));
                }
                /*south border*/
                j = uydim - 1;
                for (size_t i = 1; i < uxdim - 1; i++) {
                    if ((j % 4) == 1) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i + 1][j - 1] + uvalue[i - 1][j] + uvalue[i][j] +
                                         uvalue[i + 1][j]) / ((float) 5.0));
                    } else if ((j % 4) == 2) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i + 1][j - 1] + uvalue[i - 1][j] + uvalue[i][j] +
                                         uvalue[i + 1][j]) / ((float) 5.0));
                    } else if ((j % 4) == 3) {
                        uvalue[i][j] = ((uvalue[i - 1][j - 1] + uvalue[i][j - 1] + uvalue[i - 1][j] + uvalue[i][j] +
                                         uvalue[i + 1][j]) / ((float) 5.0));
                    } else if ((j % 4) == 0) {
                        uvalue[i][j] = ((uvalue[i - 1][j - 1] + uvalue[i][j - 1] + uvalue[i - 1][j] + uvalue[i][j] +
                                         uvalue[i + 1][j]) / ((float) 5.0));
                    }
                }
                /*east border*/
                size_t i = uxdim - 1;
                for (j = 1; j < uydim - 1; j++) {
                    if ((j % 4) == 1) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i - 1][j] + uvalue[i][j] + uvalue[i - 1][j + 1] +
                                         uvalue[i][j + 1]) / ((float) 5.0));
                    } else if ((j % 4) == 2) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i - 1][j] + uvalue[i][j] + uvalue[i][j + 1]) /
                                        ((float) 4.0));
                    } else if ((j % 4) == 3) {
                        uvalue[i][j] = ((uvalue[i - 1][j - 1] + uvalue[i][j - 1] + uvalue[i - 1][j] + uvalue[i][j] +
                                         uvalue[i][j + 1]) / ((float) 5.0));
                    } else if ((j % 4) == 0) {
                        uvalue[i][j] = ((uvalue[i - 1][j - 1] + uvalue[i][j - 1] + uvalue[i - 1][j] + uvalue[i][j] +
                                         uvalue[i - 1][j + 1] + uvalue[i][j + 1]) / ((float) 6.0));

                    }
                }
                i = 0;
                for (j = 1; j < uydim - 1; j++) {
                    /*west border*/
                    if ((j % 4) == 1) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i + 1][j - 1] + uvalue[i][j] + uvalue[i + 1][j] +
                                         uvalue[i][j + 1]) / ((float) 5.0));
                    } else if ((j % 4) == 2) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i + 1][j - 1] + uvalue[i][j] + uvalue[i + 1][j] +
                                         uvalue[i][j + 1] + uvalue[i + 1][j + 1]) / ((float) 6.0));
                    } else if ((j % 4) == 3) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i][j] + uvalue[i + 1][j] + uvalue[i][j + 1] +
                                         uvalue[i + 1][j + 1]) / ((float) 5.0));
                    } else if ((j % 4) == 0) {
                        uvalue[i][j] = ((uvalue[i][j - 1] + uvalue[i][j] + uvalue[i + 1][j] + uvalue[i][j + 1]) /
                                        ((float) 4.0));
                    }
                }

                /*Corners*/
                uvalue[0][0] = (uvalue[1][0] + uvalue[0][0] + uvalue[0][1]) / ((float) 3.0);
                uvalue[(uxdim - 1)][0] = (uvalue[(uxdim - 1)][0] + uvalue[(uxdim - 1)][1] + uvalue[(uxdim - 2)][0] +
                                          uvalue[(uxdim - 2)][1]) / ((float) 4.0);
                /* Short cut */
                uvalue[(uxdim - 1)][(uydim - 1)] =
                        (uvalue[(uxdim - 1)][(uydim - 1)] + uvalue[(uxdim - 1)][(uydim - 2)] +
                         uvalue[(uxdim - 2)][(uydim - 1)]) / ((float) 3.0);
                uvalue[0][(uydim - 1)] = (uvalue[0][(uydim - 1)] + uvalue[1][(uydim - 1)] + uvalue[0][(uydim - 2)]) /
                                         ((float) 3.0);

                for (j = 0; j < uydim; j++) {
                    for (i = 0; i < uxdim; i++) {
                        uvalue[i][j] = umat2[i][j];
                    }
                }
            }

            void medianUMatrix() {
                UMatCodes &uvalue = (*UMat<Out>::uMatrix);

                size_t xdim = UMat<Out>::xdim;
                size_t ydim = UMat<Out>::ydim;
                size_t dim = UMat<Out>::dim;
                size_t uxdim = UMat<Out>::uxdim;
                size_t uydim = UMat<Out>::uydim;

                UMatCodes umat2(uxdim, uydim); // temp

                for (size_t j = 1; j < uydim - 1; j++) {
                    for (size_t i = 1; i < uxdim - 1; i++) {
                        /* Non-borders */
                        if ((j % 4) == 1) {
                            uvalue[i][j] = (UMat<Out>::median7(uvalue[i][j - 1], uvalue[i + 1][j - 1], uvalue[i - 1][j],
                                                    uvalue[i][j], uvalue[i + 1][j], uvalue[i - 1][j + 1],
                                                    uvalue[i][j + 1]));
                        } else if ((j % 4) == 2) {
                            uvalue[i][j] = (UMat<Out>::median7(uvalue[i][j - 1], uvalue[i + 1][j - 1], uvalue[i - 1][j],
                                                    uvalue[i][j],
                                                    uvalue[i + 1][j], uvalue[i][j + 1], uvalue[i + 1][j + 1]));
                        } else if ((j % 4) == 3) {
                            uvalue[i][j] = (UMat<Out>::median7(uvalue[i - 1][j - 1], uvalue[i][j - 1], uvalue[i - 1][j],
                                                    uvalue[i][j],
                                                    uvalue[i + 1][j], uvalue[i][j + 1], uvalue[i + 1][j + 1]));
                        } else if ((j % 4) == 0) {
                            uvalue[i][j] = (UMat<Out>::median7(uvalue[i - 1][j - 1], uvalue[i][j - 1], uvalue[i - 1][j],
                                                    uvalue[i][j],
                                                    uvalue[i + 1][j], uvalue[i - 1][j + 1], uvalue[i][j + 1]));
                        }
                    }
                }

                /* north border */
                size_t j = 0;
                for (size_t i = 1; i < uxdim - 1; i++) {
                    uvalue[i][j] = (UMat<Out>::median5(uvalue[i - 1][j], uvalue[i][j], uvalue[i + 1][j], uvalue[i - 1][j + 1],
                                            uvalue[i][j + 1]));
                }
                /*south border*/
                j = uydim - 1;
                for (size_t i = 1; i < uxdim - 1; i++) {
                    if ((j % 4) == 1) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i][j - 1], uvalue[i + 1][j - 1], uvalue[i - 1][j], uvalue[i][j],
                                                uvalue[i + 1][j]));
                    } else if ((j % 4) == 2) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i][j - 1], uvalue[i + 1][j - 1], uvalue[i - 1][j], uvalue[i][j],
                                                uvalue[i + 1][j]));
                    } else if ((j % 4) == 3) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i - 1][j - 1], uvalue[i][j - 1], uvalue[i - 1][j], uvalue[i][j],
                                                uvalue[i + 1][j]));
                    } else if ((j % 4) == 0) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i - 1][j - 1], uvalue[i][j - 1], uvalue[i - 1][j], uvalue[i][j],
                                                uvalue[i + 1][j]));
                    }
                }

                // east border
                size_t i = uxdim - 1;
                for (j = 1; j < uydim - 1; j++) {
                    if ((j % 4) == 1) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i][j - 1], uvalue[i - 1][j], uvalue[i][j], uvalue[i - 1][j + 1],
                                                uvalue[i][j + 1]));
                    } else if ((j % 4) == 2) {
                        uvalue[i][j] = (UMat<Out>::median4(uvalue[i][j - 1], uvalue[i - 1][j], uvalue[i][j], uvalue[i][j + 1]));
                    } else if ((j % 4) == 3) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i - 1][j - 1], uvalue[i][j - 1], uvalue[i - 1][j], uvalue[i][j],
                                                uvalue[i][j + 1]));
                    } else if ((j % 4) == 0) {
                        uvalue[i][j] = (UMat<Out>::median6(uvalue[i - 1][j - 1], uvalue[i][j - 1], uvalue[i - 1][j], uvalue[i][j],
                                                uvalue[i - 1][j + 1], uvalue[i][j + 1]));
                    }
                }

                // west border
                i = 0;
                for (j = 1; j < uydim - 1; j++) {
                    if ((j % 4) == 1) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i][j - 1], uvalue[i + 1][j - 1], uvalue[i][j], uvalue[i + 1][j],
                                                uvalue[i][j + 1]));
                    } else if ((j % 4) == 2) {
                        uvalue[i][j] = (UMat<Out>::median6(uvalue[i][j - 1], uvalue[i + 1][j - 1], uvalue[i][j], uvalue[i + 1][j],
                                                uvalue[i][j + 1], uvalue[i + 1][j + 1]));
                    } else if ((j % 4) == 3) {
                        uvalue[i][j] = (UMat<Out>::median5(uvalue[i][j - 1], uvalue[i][j], uvalue[i + 1][j], uvalue[i][j + 1],
                                                uvalue[i + 1][j + 1]));
                    } else if ((j % 4) == 0) {
                        uvalue[i][j] = (UMat<Out>::median4(uvalue[i][j - 1], uvalue[i][j], uvalue[i + 1][j], uvalue[i][j + 1]));
                    }
                }

                // corners
                uvalue[0][0] = UMat<Out>::median3(uvalue[1][0], uvalue[0][0], uvalue[0][1]);
                uvalue[(uxdim - 1)][0] = UMat<Out>::median4(uvalue[(uxdim - 1)][0], uvalue[(uxdim - 1)][1], uvalue[(uxdim - 2)][0],
                                                 uvalue[(uxdim - 2)][1]);
                uvalue[(uxdim - 1)][(uydim - 1)] = UMat<Out>::median3(uvalue[(uxdim - 1)][(uydim - 1)],
                                                           uvalue[(uxdim - 1)][(uydim - 2)],
                                                           uvalue[(uxdim - 2)][(uydim - 1)]);
                uvalue[0][(uydim - 1)] = UMat<Out>::median3(uvalue[0][(uydim - 1)], uvalue[1][(uydim - 1)],
                                                 uvalue[0][(uydim - 2)]);

                for (size_t j = 0; j < uydim; j++) {
                    for (size_t i = 0; i < uxdim; i++) {
                        uvalue[i][j] = umat2[i][j];
                    }
                }

                UMat<Out>::scaleUMatrix(1);
            }

        };
    }
}


#endif
