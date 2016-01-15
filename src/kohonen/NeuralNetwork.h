/**
 * Kohonen neural networks. Store neural weights as matrix.
 * Each matrix row is one neural
 */

#ifndef SRC_KOHONEN_NEURALNETWORK_H
#define SRC_KOHONEN_NEURALNETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

namespace kohonen {

    template<typename T>
    class NeuralNetwork {
    public:

        NeuralNetwork() {

        }

    private:
        size_t xdim;
        size_t ydim;

    };
}



#endif
