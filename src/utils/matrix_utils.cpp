/*
 * matrix_utils.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: ilya
 */
#include "matrix_utils.h"

//template<typename T>
//utils::CMatrix<T>* inverseMatrix(const utils::CMatrix<T>& matrix) {
//
//}
namespace utils {
    template<typename T>
    utils::CMatrix<T> *multiplyMatrix(const utils::CMatrix<T> &a,
                                      const utils::CMatrix<T> &b,
                                      size_t startRowA,
                                      size_t startColA,
                                      size_t startRowB,
                                      size_t startColB,
                                      size_t rowSizeA, size_t colSizeA,
                                      size_t rowSizeB,
                                      size_t colSizeB) {

        // set default size
        if (rowSizeA == 0) {
            rowSizeA = a.getRowSize() - startRowA;
        }
        if (colSizeA == 0) {
            colSizeA = a.getColSize() - startColA;
        }
        if (rowSizeB == 0) {
            rowSizeB = b.getRowSize() - startRowB;
        }
        if (colSizeB == 0) {
            colSizeB = b.getColSize() - startColB;
        }

        size_t sizeN = colSizeA;
        if (sizeN != rowSizeB) {
            throw std::logic_error("Column size A != row size B");
        }

        size_t rowSizeC = rowSizeA;
        size_t colSizeC = colSizeB;
        utils::CMatrix<T> *c = new utils::CMatrix<T>(rowSizeC, colSizeC);

        // c(i,j) = sum(a(i,r)*b(r,j)) where r=0,1...(N-1)
        for (size_t i = 0; i < rowSizeC; ++i) {
            for (size_t j = 0; j < colSizeC; ++j) {
                T res = 0;
                for (size_t r = 0; r < sizeN; ++r) {
                    T &air = a(i + startRowA, r + startColA);
                    T &brj = b((r + startRowB), (j + startColB));
                    res = res + air * brj;
                }
                (*c)(i, j) = res;
            }
        }

        return c;
    }

    template<typename T>
    T multiplyArrays(
            const T *a, const T *b, size_t sizeArray,
            size_t startA, size_t startB) {
        T res = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            res = res + a[i + startA] * b[i + startB];
        }
        return res;
    }

    template<typename T>
    T scalarMultiplyVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA, size_t startB, size_t sizeA,
            size_t sizeB) {
        // set default size
        if (sizeA == 0) {
            sizeA = a.size();
        }
        if (sizeB == 0) {
            sizeB = b.size();
        }

        if (sizeA != sizeB) {
            throw std::logic_error("size A != size B");
        }

        T res = 0;
        for (size_t i = 0; i < sizeA; ++i) {
            T &ai = a[i + startA];
            T &bi = b[i + startB];
            res = res + ai * bi;
        }
        return multiplyArrays(a.getArray(), b.getArray(), sizeA, startA, startB);
    }

    template<typename T>
    double distanceArrays(
            const T *a, const T *b, size_t sizeArray, size_t startA,
            size_t startB) {
        double res = 0.0;
        for (size_t i = 0; i < sizeArray; ++i) {
            double diff = *(a + i + startA) - *(b + i + startB);
            res = res + diff * diff;
        }
        return pow(res, 0.5);
    }

    template<typename T>
    double distanceVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA, size_t startB, size_t sizeA,
            size_t sizeB) {
        // set default size
        if (sizeA == 0) {
            sizeA = a.size();
        }
        if (sizeB == 0) {
            sizeB = b.size();
        }

        if (sizeA != sizeB) {
            throw std::logic_error("size A != size B");
        }
        return distanceArrays(a.getArray(), b.getArray(), sizeA, startA,
                              startB);
    }

    template<typename T>
    T normArray(const T *a, size_t sizeArray, size_t startIndex) {
        T res = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            const T &ai = *(a + i + startIndex);
            res = res + std::abs(ai);
        }
        return res;
    }

    template<typename T>
    double euclideanSquaredNorm(const T *a, size_t sizeArray, size_t startIndex) {
        double res = 0.0;
        for (size_t i = 0; i < sizeArray; ++i) {
            const T &ai = *(a + i + startIndex);
            res = res + ai * ai;
        }
        return res;
    }

    template<typename T>
    double euclideanNorm(const T *a, size_t sizeArray, size_t startIndex) {
        double res = 0.0;
        for (size_t i = 0; i < sizeArray; ++i) {
            const T &ai = *(a + i + startIndex);
            res = res + ai * ai;
        }
        return pow(euclideanSquaredNorm(a, sizeArray, startIndex), 0.5);
    }

    template<typename T>
    double normEuclideanVector(const utils::CArrayList<T> &a, size_t startVec,
                               size_t sizeVec) {
        // set default size
        if (sizeVec == 0) {
            sizeVec = a.size();
        }
        return euclideanNorm(a.getArray(), sizeVec, startVec);
    }


    template<typename T>
    T normVector(const utils::CArrayList<T> &a, size_t startVec,
                 size_t sizeVec) {
        // set default size
        if (sizeVec == 0) {
            sizeVec = a.size();
        }
        return normArray(a.getArray(), sizeVec, startVec);
    }


    template<typename T>
    double cosAngelVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA, size_t startB, size_t sizeA,
            size_t sizeB) {
        double res = distanceVectors(a, b, startA, startB, sizeA, sizeB) /
                     (normVector(a, startA, sizeA) *
                      normVector(b, startB, sizeB));
        return res;
    }
}
