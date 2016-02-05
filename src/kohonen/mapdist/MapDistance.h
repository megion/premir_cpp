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

            /**
             * Передаваемые координаты bx, by, tx, ty
             * должны быть знаковыми т.к. при вычислении расстояния
             * разность может быть отрицательной.
             */
            virtual T distance(long bx, long by, long tx, long ty) = 0;

        };
    }
}


#endif
