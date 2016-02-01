#ifndef SRC_KOHONEN_WINNER_WINNERINFO_H
#define SRC_KOHONEN_WINNER_WINNERINFO_H

#include <cstdio>

#include "utils/CArrayList.h"

namespace kohonen {
    namespace winner {

        template<typename T>
        class WinnerInfo {
        public:
            WinnerInfo() {
                codeIndexes = new utils::CArrayList<size_t>();
            }

            ~WinnerInfo() {
                delete codeIndexes;
            }

            T diff; // вычисленное расстояние
            // индексы нейронов победителей
            utils::CArrayList<size_t>* codeIndexes;


        };
    }
}


#endif
