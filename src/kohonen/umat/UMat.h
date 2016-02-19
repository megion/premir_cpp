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
                    somMatrix(new SOMMatrix(xdim, ydim)),
                    uxdim(2 * xdim - 1), uydim(2 * ydim - 1),
                    uMatrix(new UMatCodes(uxdim, uydim)) {
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

            int compar(const void *first, const void *sec) {
                if (*(Out *) first < *(Out *) sec) return -1;
                else return *(Out *) first > *(Out *) sec;
            }

            void scaleUMatrix() {
                UMatCodes& uvalue = (*uMatrix);

                /* find the minimum and maximum values */
                Out max = std::numeric_limits<Out>::min();
                Out min = std::numeric_limits<Out>::max();

                for (size_t i=0; i<uxdim; i++) {
                    for (size_t j=0; j<uydim; j++) {
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
                for (size_t i=0; i<uxdim; i++) {
                    for (size_t j = 0; j < uydim; j++) {
                        uvalue[i][j] = 1.0 - (uvalue[i][j] - min) / bw;
//                      uvalue[i][j] = -uvalue[i][j]/max+1.0;
                    }
                }

            }

            virtual void buildUMatrix() = 0;
            virtual void averageUMatrix() = 0;
            virtual void medianUMatrix() = 0;

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
