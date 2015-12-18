#include <exception>
#include <stdexcept>
#include <iostream>

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
                T res;
                for (size_t r = 0; r < sizeN; ++r) {
                    T& air = a(i + startRowA, r + startColA);
                    T& brj = b((r + startRowB), (j + startColB));
                    if (r==0) {
                        res = air * brj; // init
                    } else {
                        res = res + air * brj;
                    }
                }
                (*c)(i, j) = res;
            }
        }

        return c;
    }
}