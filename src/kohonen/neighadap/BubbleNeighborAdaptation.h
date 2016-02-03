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
         *
         *
         */
        template<typename In, typename Out>
        class BubbleNeighborAdaptation : public NeighborAdaptation<In, Out> {
        public:
            BubbleNeighborAdaptation(
                    kohonen::mapdist::MapDistance<Out> *_mapDist) : mapDist(
                    _mapDist) {
            }

            void adaptation(utils::SMatrix <Out> *somCodes, In *inSampleRow) {
                std::cout << "call BubbleNeighborAdaptation" << std::endl;
            }

        private:
            kohonen::mapdist::MapDistance<Out> *mapDist;

        };
    }
}


#endif
