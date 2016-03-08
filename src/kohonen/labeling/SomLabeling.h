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

#include "utils/RDMatrix.h"
#include "models/models.h"
#include "utils/console_colors.h"

namespace kohonen {
    namespace labeling {

        template<typename InRow, typename InNum, typename Out>
        class SomLabeling {
        public:

            SomLabeling(size_t _xdim, size_t _ydim) : xdim(_xdim), ydim(_ydim) {
            }

        private:

//            utils::RDMatrix<bool, Point> InPoint;

            size_t xdim;
            size_t ydim;
        };
    }
}


#endif
