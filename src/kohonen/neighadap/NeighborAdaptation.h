#ifndef SRC_KOHONEN_NEIGHADAP_NEIGHBORADAPTATION_H
#define SRC_KOHONEN_NEIGHADAP_NEIGHBORADAPTATION_H

#include <cstdio>
#include <cmath>

namespace kohonen {
    namespace neighadap {

        /**
         * Функция подстройки соседей
         */
        template<typename In, typename Out>
        class NeighborAdaptation {
        public:

            virtual void adaptation(utils::SMatrix <Out> *somCodes,
                                    In *inSampleRow) = 0;

            void recalculateCodeVector(Out *codeVector, In *inSampleRow,
                                       size_t dim, double alpha) {
                for (size_t i = 0; i < dim; ++i) {
                    if (std::isnan(inSampleRow[i])) {
                        /* ignore vector components */
                        continue;
                    } else {
                        codeVector[i] +=
                                alpha * (inSampleRow[i] - codeVector[i]);
                    }
                }


            }

        };
    }
}


#endif
