/**
 * Обучение карты SOM
 */

#ifndef SRC_KOHONEN_SOMTRAINER_H
#define SRC_KOHONEN_SOMTRAINER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

namespace kohonen {

    template<typename InStreamReader, typename In, typename Out>
    class SomTrainer {
    public:
        SomTrainer() {
        }

        /**
         * Обучение
         */
        utils::SMatrix<Out> *training(utils::SMatrix<Out> initializedSom,
                                      size_t teachSize) {

        }

    private:

    };
}



#endif
