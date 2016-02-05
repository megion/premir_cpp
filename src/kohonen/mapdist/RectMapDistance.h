#ifndef SRC_KOHONEN_MAPDIST_RECTMAPDISTANCE_H
#define SRC_KOHONEN_MAPDIST_RECTMAPDISTANCE_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "MapDistance.h"

namespace kohonen {
    namespace mapdist {

        template<typename T>
        class RectMapDistance : public MapDistance<T> {
        public:
            RectMapDistance() {
            }

            T distance(long bx, long by, long tx, long ty) {
                T diff = bx - tx;
                T ret = diff * diff;
                diff = by - ty;
                ret += diff * diff;
                ret = std::sqrt(ret);
                return ret;
            }

        };
    }
}


#endif
