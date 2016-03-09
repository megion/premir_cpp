/**
 * Маркировка карты SOM
 */

#ifndef SRC_KOHONEN_LABELING_SOMLABELING_H
#define SRC_KOHONEN_LABELING_SOMLABELING_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/HashMapArray.h"
#include "utils/hash/HashEngine.h"
#include "models/models.h"
#include "utils/console_colors.h"

namespace kohonen {
    namespace labeling {

        template<typename InRow, typename InNum, typename Out, typename Label>
        class SomLabeling {
        public:

            SomLabeling(size_t _xdim, size_t _ydim, utils::hash::HashEngine<Label> *hashEngine)
                    : xdim(_xdim), ydim(_ydim) {
            }

        private:
            utils::HashMapArray<Label, size_t> *winnerLabels;

            size_t xdim;
            size_t ydim;
        };
    }
}


#endif
