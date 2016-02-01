#ifndef SRC_KOHONEN_WINNER_WINNERSEARCH_H
#define SRC_KOHONEN_WINNER_WINNERSEARCH_H

#include <cstdio>

#include "utils/SMatrix.h"
#include "WinnerInfo.h"

namespace kohonen {
    namespace winner {

        template<typename In, typename Out>
        class WinnerSearch {
        public:

            virtual bool search(utils::SMatrix<Out> *somCodes, In* inSampleRow,
                                WinnerInfo<Out>* winner) = 0;

        };
    }
}


#endif
