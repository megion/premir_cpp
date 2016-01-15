/**
 * Initialization kohonen neural networks
 */

#ifndef SRC_KOHONEN_NETWORKITIALIZER_H
#define SRC_KOHONEN_NETWORKITIALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <math.h>

#include "utils/SMatrix.h"

namespace kohonen {

    template<typename T>
    class NetworkInitializer {
    public:

        NetworkInitializer() {
        }

        void lineInitialization(utils::SMatrix<T>& matrix) {

        }

    private:

    };
}



#endif
