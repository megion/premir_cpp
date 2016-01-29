#ifndef SRC_KOHONEN_WINNER_WINNERSEARCH_H
#define SRC_KOHONEN_WINNER_WINNERSEARCH_H

#include <cstdio>

#include "utils/SMatrix.h"

namespace kohonen {
    namespace winner {

        template<class Out>
        class WinnerSearch {
        public:

            virtual bool search(utils::SMatrix<Out> *somCodes) = 0;

        private:

        };
    }
}


#endif
