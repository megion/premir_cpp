#ifndef SRC_KOHONEN_MAPDIST_HEXAMAPDISTANCE_H
#define SRC_KOHONEN_MAPDIST_HEXAMAPDISTANCE_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "MapDistance.h"

namespace kohonen {
    namespace mapdist {

        class HexaMapDistance : public MapDistance {
        public:
            HexaMapDistance() {
            }

            double distance(long bx, long by, long tx, long ty) {
                double diff = bx - tx;

                if (((by - ty) % 2) != 0) {
                    if ((by % 2) == 0) {
                        diff -= 0.5;
                    }
                    else {
                        diff += 0.5;
                    }
                }

                double ret = diff * diff;
                diff = by - ty;
                ret += 0.75 * diff * diff;
                ret = sqrt(ret);

                return ret;
            }

        };
    }
}


#endif
