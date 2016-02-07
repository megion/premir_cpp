#ifndef SRC_KOHONEN_WINNER_WINNERSEARCH_H
#define SRC_KOHONEN_WINNER_WINNERSEARCH_H

#include <cstdio>

#include "utils/SMatrix.h"
#include "models/DataSample.h"

namespace kohonen {
    namespace winner {

        template<typename Out>
        struct WinnerInfo {
            Out diff; // вычисленное расстояние
            long index; // индекс нейрона победителя
        };

        template<typename In, typename Out>
        class WinnerSearch {
        public:

            WinnerSearch(size_t _winnersSize) : winnersSize(_winnersSize) {
            }

            virtual bool search(utils::SMatrix<Out> *somCodes,
                                models::DataSample<In> *inSampleRow,
                                WinnerInfo<Out> *winners) = 0;

            size_t getWinnerSize() {
                return winnersSize;
            }

        protected:
            size_t winnersSize;

        };
    }
}


#endif
