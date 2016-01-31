#ifndef SRC_KOHONEN_WINNER_WINNERSEARCH_H
#define SRC_KOHONEN_WINNER_WINNERSEARCH_H

#include <cstdio>

#include "utils/SMatrix.h"

namespace kohonen {
    namespace winner {

        template<typename T>
        class WinnerSearch {
        public:

            virtual bool search(utils::SMatrix<T> *somCodes) = 0;

        };
    }
}


#endif
