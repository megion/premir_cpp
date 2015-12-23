#include <exception>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include "CMatrix.h"
#include "CArrayList.h"

namespace utils {

    /**
     * Calculate reverse matrix. Return new matrix
     */
    utils::CMatrix<double> *inverseMatrix(const utils::CMatrix<double> &matrix);

/**
 * Нахождение автокорреляционной матрицы входного вектора.
 */
//    template<typename T>
//    utils::CMatrix<T> *autocorrelationMatrix(
//            const utils::CArrayList<T> &vector) {
//
//    }

    /**
     * Умножение матрицы A на матрицу B. Умножение матриц требует,
     * чтобы размерность матриц была следующая A[MxN] B[NxQ] и результат
     * умножения матрица C=A*B будет иметь размер C[MxQ].
     */
    CMatrix<double> *multiplyMatrix(const CMatrix<double> &a,
                                    const CMatrix<double> &b,
                                    size_t startRowA,
                                    size_t startColA,
                                    size_t startRowB,
                                    size_t startColB,
                                    size_t rowSizeA,
                                    size_t colSizeA,
                                    size_t rowSizeB,
                                    size_t colSizeB);

    /**
     * Скаларное произведение массивов A и B.
     * result = sum(a(i)*b(i)) where i = 0...N-1
     */
    double multiplyArrays(
            const double *a, const double *b, size_t sizeArray);

    /**
     * Расстояние между массивами A и B.
     *
     * result = pow(sum(pow(a(i)-b(i), 2)), 0.5) where i = 0...N-1
     */
    double distanceArrays(const double *a, const double *b, size_t sizeArray);

    /**
     * Вычисление нормы массива
     * result = sum(|a(i)|) where i = 0...N-1
     */
    double normArray(const double *a, size_t sizeArray);

    /**
     * Вычисление квадрата евклидовой нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
    double euclideanSquaredNorm(const double *a, size_t sizeArray);

    /**
     * Вычисление евклидовой нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
    double euclideanNorm(const double *a, size_t sizeArray);


    /**
     * Расстояние между векторами A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
     *
     * result = distance(x,y)/(|x|*|y|)
     */
//    template<typename T>
//    double cosAngelVectors(
//            const utils::CArrayList<double> &a,
//            const utils::CArrayList<double> &b,
//            size_t startA = 0, size_t startB = 0, size_t sizeA = 0,
//            size_t sizeB = 0);

}
