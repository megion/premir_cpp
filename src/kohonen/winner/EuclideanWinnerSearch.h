#ifndef SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H
#define SRC_KOHONEN_WINNER_EUCLIDEANWINNERSEARCH_H

#include <cstdio>
#include <iostream>

#include "utils/SMatrix.h"
#include "WinnerSearch.h"

namespace kohonen {
    namespace winner {

        template<typename T>
        class EuclideanWinnerSearch : public WinnerSearch<T> {
        public:
            EuclideanWinnerSearch() {
            }

            bool search(utils::SMatrix<T> *somCodes) {
                std::cout << "call EuclideanWinnerSearch" << std::endl;
                return true;
            }

        private:

        };
    }
}


#endif
