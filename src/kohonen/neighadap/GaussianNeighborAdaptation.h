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
         *
         */
        template<typename In, typename Out>
        class GaussianNeighborAdaptation : public NeighborAdaptation<In, Out> {
        public:

            GaussianNeighborAdaptation(
                    kohonen::mapdist::MapDistance<Out> *_mapDist,
                    size_t _xdim, size_t _ydim)
                    : mapDist(_mapDist),
                      NeighborAdaptation<In, Out>(_xdim, _ydim) {
            }

            /**
             * Adaptation function for gaussian neighbourhood
             */
            void adaptation(utils::SMatrix<Out> *somCodes, In *inSampleRow,
                            long bx, long by,
                            Out radius, Out alpha) {
                for (size_t r=0; r<somCodes->getRowSize(); ++r) {
                    long tx = r % NeighborAdaptation<In, Out>::xdim;
                    long ty = r / NeighborAdaptation<In, Out>::xdim;

                    Out dd = mapDist->distance(bx, by, tx, ty);
                    Out alp = alpha *
                          (Out) std::exp((double) (-dd * dd / (2.0 * radius * radius)));
                    NeighborAdaptation<In, Out>::recalculateCodeVector(
                            somCodes->getRow(r),
                            inSampleRow, somCodes->getColSize(), alp);
                }
            }

        };
    }
}


#endif
