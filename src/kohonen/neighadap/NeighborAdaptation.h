#ifndef SRC_KOHONEN_NEIGHADAP_NEIGHBORADAPTATION_H
#define SRC_KOHONEN_NEIGHADAP_NEIGHBORADAPTATION_H

#include <cstdio>
#include <cmath>

#include "utils/SMatrix.h"

namespace kohonen {
    namespace neighadap {

        /**
         * Функция подстройки соседей
         */
        template<typename In, typename Out>
        class NeighborAdaptation {
        public:

            NeighborAdaptation(size_t _xdim, size_t _ydim) :
                    xdim(_xdim), ydim(_ydim) {
            }

            virtual void adaptation(utils::SMatrix<Out> *somCodes,
                                    In *inSampleRow, long bx, long by,
                                    Out radius, Out alpha) = 0;

            void recalculateCodeVector(Out *codeVector, In *inSampleRow,
                                       size_t dim, Out alpha) {
                for (size_t i = 0; i < dim; ++i) {
                    // TODO: ignore NAN vector components
                    if (!std::isnan(inSampleRow[i])) {
                        codeVector[i] +=
                                alpha * (inSampleRow[i] - codeVector[i]);
                    }
                }
            }

        protected:
            size_t xdim;
            size_t ydim;

        };
    }
}


#endif
