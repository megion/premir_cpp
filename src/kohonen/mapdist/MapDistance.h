#ifndef SRC_KOHONEN_MAPDIST_MAPDISTANCE_H
#define SRC_KOHONEN_MAPDIST_MAPDISTANCE_H

#include <cstdio>

namespace kohonen {
    namespace mapdist {

        /**
         * Вычисление расстояния на карте между двумя точками (нейронами)
         * на карте. Вычисление зависит от топологии карты (HEXA или RECT).
         */
        template<typename T>
        class MapDistance {
        public:

            virtual T distance(size_t bx, size_t by, size_t tx, size_t ty) = 0;

        };
    }
}


#endif
