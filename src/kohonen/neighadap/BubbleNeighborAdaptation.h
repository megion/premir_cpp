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
        template<typename In, typename Out>
        class BubbleNeighborAdaptation : public NeighborAdaptation<In, Out> {
        public:
            BubbleNeighborAdaptation(
                    kohonen::mapdist::MapDistance<Out> *_mapDist,
                    size_t _xdim, size_t _ydim)
                    : mapDist(_mapDist),
                      NeighborAdaptation<In, Out>(_xdim, _ydim) {
            }

            void adaptation(utils::SMatrix<Out> *somCodes,
                            models::DataSample<In> *inSampleRow,
                            long bx, long by,
                            Out radius, Out alpha) {
                for (size_t r = 0; r < somCodes->getRowSize(); ++r) {
                    long tx = r % NeighborAdaptation<In, Out>::xdim;
                    long ty = r / NeighborAdaptation<In, Out>::xdim;

                    if ((mapDist->distance(bx, by, tx, ty)) <= radius) {
                        NeighborAdaptation<In, Out>::recalculateCodeVector(
                                somCodes->getRow(r),
                                inSampleRow, somCodes->getColSize(), alpha);
                    }
                }

            }

        private:
            kohonen::mapdist::MapDistance<Out> *mapDist;

        };
    }
}


#endif
