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
//    template<typename T>
//    utils::CMatrix<T> *inverseMatrix(const utils::CMatrix<T> &matrix) {
//    }

/**
 * Нахождение автокорреляционной матрицы входного вектора.
 */
//    template<typename T>
//    utils::CMatrix<T> *autocorrelationMatrix(
//            const utils::CArrayList<T> &vector) {
//
//    }

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
                                      size_t colSizeB = 0);

    /**
     * Скаларное произведение массивов A и B.
     * result = sum(a(i)*b(i)) where i = 0...N-1
     */
    template<typename T>
    T multiplyArrays(
            const T *a, const T *b, size_t sizeArray,
            size_t startA = 0, size_t startB = 0);

    /**
     * Скаларное произведение вектров A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
     */
    template<typename T>
    T scalarMultiplyVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA = 0, size_t startB = 0, size_t sizeA = 0,
            size_t sizeB = 0);

    /**
     * Расстояние между массивами A и B.
     *
     * result = pow(sum(pow(a(i)-b(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double distanceArrays(
            const T *a, const T *b, size_t sizeArray, size_t startA = 0,
            size_t startB = 0);

    /**
     * Расстояние между векторами A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
     */
    template<typename T>
    double distanceVectors(
            const utils::CArrayList<T> &a, const utils::CArrayList<T> &b,
            size_t startA = 0, size_t startB = 0, size_t sizeA = 0,
            size_t sizeB = 0);

    /**
     * Вычисление нормы массива
     * result = sum(|a(i)|) where i = 0...N-1
     */
    template<typename T>
    T normArray(const T *a, size_t sizeArray, size_t startIndex = 0);

    /**
     * Вычисление квадрата евклидовой нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double euclideanSquaredNorm(const T *a, size_t sizeArray,
                                size_t startIndex = 0);

    /**
     * Вычисление евклидовой нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double euclideanNorm(const T *a, size_t sizeArray, size_t startIndex = 0);


    /**
     * Вычисление евклидовой нормы вектора
     */
    template<typename T>
    double normEuclideanVector(const utils::CArrayList<T> &a,
                               size_t startVec = 0,
                               size_t sizeVec = 0);

    /**
     * Вычисление нормы вектора
     */
    template<typename T>
    T normVector(const utils::CArrayList<T> &a, size_t startVec = 0,
                 size_t sizeVec = 0);


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
            size_t sizeB = 0);

    /**
     * Нахождение ортигонального векторного базиса по методу Грама — Шмидта
     * (последовательность векторов h(i)) в соответствии с рекурсивным
     * правилом
     * h(i) = x(i) - sum(distance(x(i), h(j)) * h(j) / (pow(|h(j)|, 2)))
     * where j = 0 ... i-1
     *
     */
//    template<typename T>
//    utils::CMatrix<T> *gramSchmidtVectorBasis(const utils::CMatrix<T> &a,
//                                             size_t startRow = 0,
//                                             size_t startCol = 0,
//                                             size_t rowSize = 0,
//                                             size_t colSize = 0) {
//        if (a.getRowSize() == 0) {
//            throw std::logic_error("Row size == 0");
//        }
//
//        // set default size
//        if (rowSize == 0) {
//            rowSize = a.getRowSize() - startRow;
//        }
//        if (colSize == 0) {
//            colSize = a.getColSize() - startCol;
//        }
//
//        // ортогональные вектора в виде матрицы где вектора расположены
//        // по строкам
//        utils::CMatrix<T> *vectors = new utils::CMatrix<T>(0, colSize);
//        utils::CArrayList<double> rowNormos = utils::CArrayList<double>(
//                rowSize);
//
//        size_t rowIndex = 0;
//        for (size_t i = 0; i < rowSize; ++i) {
//            const T *x = a.getRowArray(i + startRow) + startCol;
//
//            if (rowIndex==0) {
//
//            }
//            double normX = normEuclideanArray(x, colSize);
//            if (normX != 0) {
//                // добавляем и обрабатываем только если норма вектора != 0
//
//                if (rowIndex == 0) {
//                    // начальное значение ортогонального вектора инициализуется
//                    // первым вектором норма которого !=0
//                    vectors->pushRow(x);
//                    rowNormos.push(normX);
//                } else {
//                    // вычисление по предыдущим ортогональным векторам
//                    utils::CArrayList<double> tempSum = utils::CArrayList<double>(
//                            colSize);
//                    for (size_t vectorIndex = 0;
//                         vectorIndex < vectors->getRowSize(); ++vectorIndex) {
//                        const T *h = vectors->getRowArray(vectorIndex);
//                        std::cout << "h [" << *h << ", " << *(h+1) << ", " << *(h+2) << "] " << std::endl;
//                        std::cout << "x [" << *x << ", " << *(x+1) << ", " << *(x+2) << "] " << std::endl;
//                        double norm = rowNormos[vectorIndex];
//                        std::cout << "norm " << norm << std::endl;
//                        double m = multiplyArrays(x, h, colSize);
//                        std::cout << "m " << m << std::endl;
//                        double k = m / (norm * norm);
//
//                        std::cout << "k [" << rowIndex << "]: "<< k << std::endl;
//
//                        utils::CArrayList<double> temp = utils::CArrayList<double>(
//                                colSize);
//                        temp.write(0, h, colSize);
//                        temp.multiply(k);
////                        temp.print();
//                        if (vectorIndex == 0) {
//                            tempSum.write(0, temp.getArray(), colSize); // init
//                        } else {
//                            tempSum.sum(temp); // sum
//                        }
//                        tempSum.print();
//                    }
//
//                    tempSum.reverseMinus(x);
//                    std::cout << "tempSum" << std::endl;
//                    tempSum.print();
//
//                    vectors->pushRow(tempSum.getArray());
//                }
//                ++rowIndex;
//            }
//        }
//
//        return vectors;
//    }
}