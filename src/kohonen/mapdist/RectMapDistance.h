#ifndef SRC_KOHONEN_MAPDIST_RECTMAPDISTANCE_H
#define SRC_KOHONEN_MAPDIST_RECTMAPDISTANCE_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "MapDistance.h"
#include "utils/R3DMatrix.h"

namespace kohonen {
    namespace mapdist {

        class RectMapDistance : public MapDistance {
        public:
            RectMapDistance() {
            }

            double distance(long bx, long by, long tx, long ty) {
                double diff = bx - tx;
                double ret = diff * diff;
                diff = by - ty;
                ret += diff * diff;
                ret = std::sqrt(ret);
                return ret;
            }

        };
    }
}


#endif
