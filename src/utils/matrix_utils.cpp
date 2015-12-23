/*
 * matrix_utils.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: ilya
 */
#include "matrix_utils.h"

namespace utils {

    CMatrix<double> *inverseMatrix(const CMatrix<double> &matrix) {
        CMatrix<double> *aM = matrix.createClone();
        CMatrix<double> *iM = new CMatrix<double>(aM->getRowSize(),
                                                  aM->getColSize());
        iM->makeIdentity();

        // поиском по столбцам найдем первый отличный от 0 элемент
        for (size_t c = 0; c < aM->getColSize(); ++c) {
            for (size_t r = 0; r < aM->getRowSize(); ++r) {
                double &val = (*aM)(r, c);
                if (val != 0) {
                    // найден не нулевой элемент столбца
                    if (r != 0) {
                        // if not first row then swap rows
                        aM->swapRows(0, r);
                    }
                }
            }
        }
        return iM;
    }

    CMatrix<double> *multiplyMatrix(const CMatrix<double> &a,
                                    const CMatrix<double> &b,
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
        CMatrix<double> *c = new CMatrix<double>(rowSizeC, colSizeC);

        // c(i,j) = sum(a(i,r)*b(r,j)) where r=0,1...(N-1)
        for (size_t i = 0; i < rowSizeC; ++i) {
            for (size_t j = 0; j < colSizeC; ++j) {
                double res = 0;
                for (size_t r = 0; r < sizeN; ++r) {
                    double &air = a(i + startRowA, r + startColA);
                    double &brj = b((r + startRowB), (j + startColB));
                    res = res + air * brj;
                }
                (*c)(i, j) = res;
            }
        }

        return c;
    }

    double multiplyArrays(
            const double *a, const double *b, size_t sizeArray) {
        double res = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            res = res + a[i] * b[i];
        }
        return res;
    }

    double distanceArrays(
            const double *a, const double *b, size_t sizeArray) {
        double res = 0.0;
        for (size_t i = 0; i < sizeArray; ++i) {
            double diff = a[i] - b[i];
            res = res + diff * diff;
        }
        return pow(res, 0.5);
    }

    double normArray(const double *a, size_t sizeArray) {
        double res = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            const double &ai = *(a + i);
            res = res + std::abs(ai);
        }
        return res;
    }

    double euclideanSquaredNorm(const double *a, size_t sizeArray) {
        double res = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            const double &ai = *(a + i);
            res = res + ai * ai;
        }
        return res;
    }

    double euclideanNorm(const double *a, size_t sizeArray) {
        return pow(euclideanSquaredNorm(a, sizeArray), 0.5);
    }

//    template<typename T>
//    double cosAngelVectors(
//            const utils::CArrayList<double> &a,
//            const utils::CArrayList<double> &b,
//            size_t startA, size_t startB, size_t sizeA,
//            size_t sizeB) {
//        double res = distanceVectors(a, b, startA, startB, sizeA, sizeB) /
//                     (normVector(a, startA, sizeA) *
//                      normVector(b, startB, sizeB));
//        return res;
//    }
}
