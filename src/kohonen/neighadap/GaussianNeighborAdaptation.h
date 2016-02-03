#ifndef SRC_KOHONEN_NEIGHADAP_GAUSSIANNEIGHBORADAPTATION_H
#define SRC_KOHONEN_NEIGHADAP_GAUSSIANNEIGHBORADAPTATION_H

#include <cstdio>
#include <iostream>

#include "NeighborAdaptation.h"

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
            GaussianNeighborAdaptation() {
            }

            void adaptation(utils::SMatrix<Out> *somCodes, In* inSampleRow) {
                std::cout << "call GaussianNeighborAdaptation" << std::endl;
            }

        };
    }
}


#endif
