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
//    template<typename T>
    utils::CMatrix<double> *multiplyMatrix(const utils::CMatrix<double> &a,
                                           const utils::CMatrix<double> &b,
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
//    template<typename T>
    double multiplyArrays(
            const double *a, const double *b, size_t sizeArray);

    /**
     * Расстояние между массивами A и B.
     *
     * result = pow(sum(pow(a(i)-b(i), 2)), 0.5) where i = 0...N-1
     */
//    template<typename T>
    double distanceArrays(const double *a, const double *b, size_t sizeArray);

    /**
     * Вычисление нормы массива
     * result = sum(|a(i)|) where i = 0...N-1
     */
//    template<typename T>
    double normArray(const double *a, size_t sizeArray);

    /**
     * Вычисление квадрата евклидовой нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
//    template<typename T>
    double euclideanSquaredNorm(const double *a, size_t sizeArray);

    /**
     * Вычисление евклидовой нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
//    template<typename T>
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
