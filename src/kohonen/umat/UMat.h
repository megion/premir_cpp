#ifndef SRC_KOHONEN_UMAT_UMAT_H
#define SRC_KOHONEN_UMAT_UMAT_H

#include <cstdio>

#include "utils/RMatrix.h"
#include "models/DataSample.h"

namespace kohonen {
    namespace umat {

        /**
         * Построение U-матрицы для SOM. Вычисление зависит от топологии карты (HEXA или RECT).
         */
        template<typename Out>
        class UMat {
        public:

            typedef utils::RMatrix<models::NeuronInfo, Out> SomCodes;
            typedef utils::RMatrix<models::NeuronInfo, Out> UMatCodes;
            typedef utils::R3DMatrix<bool, models::NeuronInfo, Out> SOMMatrix;

            UMat(size_t _xdim, size_t _ydim, size_t _dim) :
                    xdim(_xdim), ydim(_ydim), dim(_dim),
                    uxdim(2 * xdim - 1), uydim(2 * ydim - 1) {
                somMatrix = new SOMMatrix(xdim, ydim);
                uMatrix = new UMatCodes(uxdim, uydim);
            }

            ~UMat() {
                delete somMatrix;
                delete uMatrix;
            }

            void initializeMat(SomCodes *trainedSom) {
                size_t neuronIndex = 0;
                for (size_t j = 0; j < ydim; ++j) {
                    for (size_t i = 0; i < xdim; ++i) {
                        // write element to [i][j]
                        somMatrix->writePoints(i, j, trainedSom->getRowPoints(neuronIndex), dim);
                        ++neuronIndex;
                    }
                }
            }

            UMatCodes* getUMatrix() {
                return uMatrix;
            }

            int compar(const void *first, const void *sec) {
                if (*(Out *) first < *(Out *) sec) return -1;
                else return *(Out *) first > *(Out *) sec;
            }

            void scaleUMatrix(int type = 0) {
                UMatCodes &uvalue = (*uMatrix);

                /* find the minimum and maximum values */
                Out max = std::numeric_limits<Out>::min();
                Out min = std::numeric_limits<Out>::max();

                for (size_t i = 0; i < uxdim; i++) {
                    for (size_t j = 0; j < uydim; j++) {
                        if (uvalue[i][j] > max) {
                            max = uvalue[i][j];
                        }
                        if (uvalue[i][j] < min) {
                            min = uvalue[i][j];
                        }
                    }
                }

                std::cout << "minimum distance between elements: " << min << std::endl;
                std::cout << "maximum distance between elements: " << max << std::endl;

                Out bw = max - min;
                /* scale values to [0,1] */
                for (size_t i = 0; i < uxdim; i++) {
                    for (size_t j = 0; j < uydim; j++) {
                        if (type == 0) {
                            uvalue[i][j] = 1.0 - (uvalue[i][j] - min) / bw;
                        } else if (type == 1) {
                            uvalue[i][j] = uvalue[i][j] / max;
                        } else if (type == 2) {
                            uvalue[i][j] = -uvalue[i][j] / max + 1.0;
                        }
                    }
                }

            }

            virtual void buildUMatrix() = 0;

            virtual void averageUMatrix() = 0;

            virtual void medianUMatrix() = 0;

            ////////////////////////////////////////////////////
            // TODO: should remove by ArrayUtils.median
            Out median7(Out yy, Out kaa, Out koo, Out nee, Out vii, Out kuu, Out see) {
                Out array[7];
                Out tmp;
                int i, j;

                array[0] = yy;
                array[1] = kaa;
                array[2] = koo;
                array[3] = nee;
                array[4] = vii;
                array[5] = kuu;
                array[6] = see;

                for (j = 0; j < 7 - 1; j++)
                    for (i = 0; i < 7 - 1 - j; i++) {
                        if (array[i] > array[i + 1]) {
                            tmp = array[i];
                            array[i] = array[i + 1];
                            array[i + 1] = tmp;
                        }
                    }
                return (array[3]);
            }

            Out median6(Out yy, Out kaa, Out koo, Out nee, Out vii, Out kuu) {
                Out array[6];
                Out tmp;
                int i, j;

                array[0] = yy;
                array[1] = kaa;
                array[2] = koo;
                array[3] = nee;
                array[4] = vii;
                array[5] = kuu;

                for (j = 0; j < 6 - 1; j++)
                    for (i = 0; i < 6 - 1 - j; i++) {
                        if (array[i] > array[i + 1]) {
                            tmp = array[i];
                            array[i] = array[i + 1];
                            array[i + 1] = tmp;
                        }
                    }
                return (array[3]);
            }

            Out median5(Out yy, Out kaa, Out koo, Out nee, Out vii) {
                Out array[5];
                Out tmp;
                int i, j;

                array[0] = yy;
                array[1] = kaa;
                array[2] = koo;
                array[3] = nee;
                array[4] = vii;


                for (j = 0; j < 5 - 1; j++)
                    for (i = 0; i < 5 - 1 - j; i++) {
                        if (array[i] > array[i + 1]) {
                            tmp = array[i];
                            array[i] = array[i + 1];
                            array[i + 1] = tmp;
                        }
                    }
                return (array[2]);
            }

            Out median4(Out yy, Out kaa, Out koo, Out nee) {
                Out array[4];
                Out tmp;
                int i, j;

                array[0] = yy;
                array[1] = kaa;
                array[2] = koo;
                array[3] = nee;


                for (j = 0; j < 4 - 1; j++)
                    for (i = 0; i < 4 - 1 - j; i++) {
                        if (array[i] > array[i + 1]) {
                            tmp = array[i];
                            array[i] = array[i + 1];
                            array[i + 1] = tmp;
                        }
                    }
                return (array[2]);
            }


            Out median3(Out yy, Out kaa, Out koo) {
                Out array[3];
                Out tmp;
                int i, j;

                array[0] = yy;
                array[1] = kaa;
                array[2] = koo;


                for (j = 0; j < 3 - 1; j++)
                    for (i = 0; i < 3 - 1 - j; i++) {
                        if (array[i] > array[i + 1]) {
                            tmp = array[i];
                            array[i] = array[i + 1];
                            array[i + 1] = tmp;
                        }
                    }
                return (array[1]);
            }


        protected:
            // пердставление списка нейронов SOM как матрицы [xdim, ydim]
            SOMMatrix *somMatrix;
            // U-матрица
            UMatCodes *uMatrix;

            size_t xdim;
            size_t ydim;
            size_t dim;
            size_t uxdim;
            size_t uydim;
        };
    }
}


#endif
