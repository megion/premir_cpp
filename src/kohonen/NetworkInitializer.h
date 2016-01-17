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

    template<typename T, typename R>
    class NetworkInitializer {
    public:

        NetworkInitializer() {
        }

        /**
         * Линейная инициализация нейронной сети.
         * Вначале определяются два собственных вектора с наибольшими
         * собствннными значениями. Затем определяется двумерная матрица
         * центр тяжести которой совпадает со средним значением входных данных.
         * Значения данной матрицы это начальные значения нейроной сети m_i_(0).
         *
         * Полсле этого можно продолжать обучение с фазы сходимости.
         *
         */
        utils::SMatrix<R>* lineInitialization(utils::SMatrix<T>& inMatrix) {
            size_t n = inMatrix.getColSize();

            R u[2*n];
            R v[2*n];
            R mu[2];

            R r[n*n];
            for (size_t i=0; i<n*n; ++i) {
                r[i]=0;
            }

            R m[n];
            for (size_t i=0; i<n; ++i) {
                m[i]=0;
            }

            long k2[n];
            for (size_t i=0; i<n; ++i) {
                k2[i]=0;
            }

            // iterate by all input matrix elements
            for (size_t k = 0; k<inMatrix.getRowSize(); ++k) {
                T* row = inMatrix.getRow(k);
                for (size_t i=0; i<n; ++i) {
                    m[i]+=row[i];
                    k2[i]++;
                }
            }

            for (size_t i=0; i<n; ++i) {
                m[i]/=k2[i];
            }

            // iterate by all input matrix elements
            for (size_t k = 0; k<inMatrix.getRowSize(); ++k) {
                T* row = inMatrix.getRow(k);
                for (size_t i=0; i<n; ++i) {
                    for (size_t j=i; j<n; j++) {
                        r[i*n+j]+=(row[i]-m[i])*(row[j]-m[j]);
                    }
                }
            }

            return nullptr;
        }

        /**
         * Случайная инициализация нейронной сети.
         */
        utils::SMatrix<R>* randomInitialization(utils::SMatrix<T>& inMatrix) {

        }


    private:

    };
}



#endif
