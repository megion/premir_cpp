#ifndef SRC_KOHONEN_NEIGHADAP_GAUSSIANNEIGHBORADAPTATION_H
#define SRC_KOHONEN_NEIGHADAP_GAUSSIANNEIGHBORADAPTATION_H

#include <cstdio>
#include <iostream>

#include "NeighborAdaptation.h"
#include "kohonen/mapdist/MapDistance.h"

namespace kohonen {
    namespace neighadap {

        /**
         * Функция адаптация соседей Gaussian
         *
         */
        class GaussianNeighborAdaptation : public NeighborAdaptation {
        public:

            GaussianNeighborAdaptation(kohonen::mapdist::MapDistance *_mapDist, size_t _xdim, size_t _ydim)
                    : mapDist(_mapDist), NeighborAdaptation(_xdim, _ydim) {
            }

            /**
             * Adaptation function for gaussian neighbourhood
             */
            void adaptation(utils::RMatrix<models::NeuronInfo, double> *somCodes, models::DataSample *inSampleRow,
                            long bx, long by, double radius, double alpha) {
                for (size_t r = 0; r < somCodes->getRowSize(); ++r) {
                    long tx = r % NeighborAdaptation::xdim;
                    long ty = r / NeighborAdaptation::xdim;

                    double dd = mapDist->distance(bx, by, tx, ty);
                    double alp = alpha * std::exp((-dd * dd / (2.0 * radius * radius)));
                    NeighborAdaptation::recalculateCodeVector(somCodes->getRow(r).points, inSampleRow,
                                                              somCodes->getColSize(), alp);
                }
            }

        private:
            kohonen::mapdist::MapDistance *mapDist;

        };
    }
}


#endif
