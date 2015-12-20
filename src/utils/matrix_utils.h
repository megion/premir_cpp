#include <exception>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include "CMatrix.h"
#include "CArrayList.h"

namespace utils {
/**
 * Нахождение обратной матрицы. Возвращается новая матрица,
 * переданная матрица не меняется.
 */
    template<typename T>
    utils::CMatrix<T> *inverseMatrix(const utils::CMatrix<T> &matrix) {
    }

/**
 * Нахождение автокорреляционной матрицы входного вектора.
 */
    template<typename T>
    utils::CMatrix<T> *autocorrelationMatrix(
            const utils::CArrayList<T> &vector) {

    }

/**
 * Умножение матрицы A на матрицу B. Умножение матриц требует чтобы размерность
 * матриц была следующая A[MxN] B[NxQ] и результат умножения матрица C=A*B
 * будет иметь размер C[MxQ]. Если число столбцов матрицы A не равно числу строк
 * матрицы B то вычисление не возможно и будет выброшена ошибка и поэтому
 * чтобы всетаки иметь возможность умножения таких матриц можно задать диапазон
 * строчек и колонок по которым будет происходить вычисление
 * (эти параметры опциональны).
 */
    template<typename T>
    utils::CMatrix<T> *multiplyMatrix(const utils::CMatrix<T> &a,
                                      const utils::CMatrix<T> &b,
                                      size_t startRowA = 0,
                                      size_t startColA = 0,
                                      size_t startRowB = 0,
                                      size_t startColB = 0,
                                      size_t rowSizeA = 0, size_t colSizeA = 0,
                                      size_t rowSizeB = 0,
                                      size_t colSizeB = 0) {

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

    /**
     * Скаларное произведение вектров A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
     * result = sum(a(i)*b(i)) where i = 0...N-1
     */
    template<typename T>
    T scalarMultiplyVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA = 0, size_t startB = 0, size_t sizeA = 0,
            size_t sizeB = 0) {
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
        return res;
    }

    /**
     * Расстояние между векторами A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
     *
     * result = pow(sum(pow(a(i)-b(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double distanceVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA = 0, size_t startB = 0, size_t sizeA = 0,
            size_t sizeB = 0) {
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

        double res = 0;
        for (size_t i = 0; i < sizeA; ++i) {
            T &ai = a[i + startA];
            T &bi = b[i + startB];
            T diff = ai - bi;
            res = res + diff * diff;
        }
        return pow(res, 0.5);
    }

    /**
     * Вычисление нормы вектора
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double normVector(const utils::CArrayList<T> &a, size_t startVec = 0,
                      size_t sizeVec = 0) {
        // set default size
        if (sizeVec == 0) {
            sizeVec = a.size();
        }
        double res = 0.0;
        for (size_t i = 0; i < sizeVec; ++i) {
            T &ai = a[i + startVec];
            res = res + ai * ai;
        }
        return pow(res, 0.5);
    }

    /**
     * Расстояние между векторами A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
     *
     * result = distance(x,y)/(|x|*|y|)
     */
    template<typename T>
    double cosAngelVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA = 0, size_t startB = 0, size_t sizeA = 0,
            size_t sizeB = 0) {
        double res = distanceVectors(a, b, startA, startB, sizeA, sizeB) /
                     (normVector(a, startA, sizeA) *
                      normVector(b, startB, sizeB));
        return res;
    }

    /**
     * Нахождение ортигонального векторного базиса
     * (последовательность векторов h(i)) в соответствии с рекурсивным
     * правилом
     * h(i) = x(i) - sum(distance(x(i), h(j)) * h(j) / (pow(|h(j)|, 2)))
     * where j = 0 ... i
     *
     */
    template<typename T>
    utils::CMatrix<T> *orthogonalVectorBasis(const utils::CMatrix<T> &a,
                                      size_t startRow = 0,
                                      size_t startCol = 0,
                                      size_t rowSize = 0, size_t colSize = 0) {

        if (a.getRowSize()==0) {
            throw std::logic_error("Row size B == 0");
        }

        // set default size
        if (rowSize == 0) {
            rowSize = a.getRowSize() - startRow;
        }
        if (colSize == 0) {
            colSize = a.getColSize() - startCol;
        }

        // ортогональные вектора в виде матрицы где вектора расположены
        // по строкам
        utils::CMatrix<T> *vectors = new utils::CMatrix<T>(0, colSize);


        for (size_t i = 0; i < rowSize; ++i) {
            for (size_t j = 0; j < colSize; ++j) {
                T res = 0;

//                for (size_t r = 0; r < sizeN; ++r) {
//                    T &air = a(i + startRowA, r + startColA);
//                    T &brj = b((r + startRowB), (j + startColB));
//                    res = res + air * brj;
//                }
//                (*c)(i, j) = res;
            }
        }

        return vectors;
    }
}