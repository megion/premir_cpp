#ifndef SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H
#define SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H

#include <cstdio>
#include <iostream>

#include "utils/SMatrix.h"
#include "WinnerSearch.h"

namespace kohonen {
    namespace winner {

        template<class Out>
        class EuclideanWinnerSearch : public WinnerSearch<Out> {
        public:
            EuclideanWinnerSearch() {
            }

            bool search(utils::SMatrix<Out> *somCodes) {
                std::cout << "call EuclideanWinnerSearch" << std::endl;
                return true;
            }

        private:

        };
    }
}


#endif
