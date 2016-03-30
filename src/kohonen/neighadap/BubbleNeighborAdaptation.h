#ifndef SRC_KOHONEN_NEIGHADAP_BUBBLENEIGHBORADAPTATION_H
#define SRC_KOHONEN_NEIGHADAP_BUBBLENEIGHBORADAPTATION_H

#include <cstdio>
#include <iostream>

#include "NeighborAdaptation.h"
#include "kohonen/mapdist/MapDistance.h"

namespace kohonen {
    namespace neighadap {

        /**
         * Функция адаптация соседей bubble
         */
        class BubbleNeighborAdaptation : public NeighborAdaptation {
        public:
            BubbleNeighborAdaptation(kohonen::mapdist::MapDistance *_mapDist, size_t _xdim, size_t _ydim)
                    : mapDist(_mapDist), NeighborAdaptation(_xdim, _ydim) {
            }

            void adaptation(utils::RMatrix<models::NeuronInfo, double> *somCodes, models::DataSample *inSampleRow,
                            long bx, long by, double radius, double alpha) {
                for (size_t r = 0; r < somCodes->getRowSize(); ++r) {
                    long tx = r % NeighborAdaptation::xdim;
                    long ty = r / NeighborAdaptation::xdim;

                    if ((mapDist->distance(bx, by, tx, ty)) <= radius) {
                        NeighborAdaptation::recalculateCodeVector(somCodes->getRow(r).points, inSampleRow,
                                                                  somCodes->getColSize(), alpha);
                    }
                }

            }

        private:
            kohonen::mapdist::MapDistance *mapDist;

        };
    }
}


#endif
