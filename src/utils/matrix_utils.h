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
     * Расстояние между массивами A и B.
     *
     * result = pow(sum(pow(a(i)-b(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double distanceArrays(
            const T *a, const T *b, size_t sizeArray, size_t startA = 0,
            size_t startB = 0) {
        double res = 0.0;
        for (size_t i = 0; i < sizeArray; ++i) {
            double diff = *(a + i + startA) - *(b + i + startB);
            res = res + diff * diff;
        }
        return pow(res, 0.5);
    }

    /**
     * Расстояние между векторами A и B. Размеры векторов должны совпадать.
     * Если не совпадают то можно задать дипазон для кажого из вектора.
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
        return distanceArrays(a.getArray(), b.getArray(), sizeA, startA,
                              startB);
    }

    /**
     * Вычисление нормы массива
     * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
     */
    template<typename T>
    double normArray(const T *a, size_t sizeArray, size_t startIndex = 0) {
        double res = 0.0;
        for (size_t i = 0; i < sizeArray; ++i) {
            const T &ai = *(a + i + startIndex);
            res = res + ai * ai;
        }
        return pow(res, 0.5);
    }

    /**
     * Вычисление нормы вектора
     */
    template<typename T>
    double normVector(const utils::CArrayList<T> &a, size_t startVec = 0,
                      size_t sizeVec = 0) {
        // set default size
        if (sizeVec == 0) {
            sizeVec = a.size();
        }
        return normArray(a.getArray(), sizeVec, startVec);
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
     * Нахождение ортигонального векторного базиса по методу Грама — Шмидта
     * (последовательность векторов h(i)) в соответствии с рекурсивным
     * правилом
     * h(i) = x(i) - sum(distance(x(i), h(j)) * h(j) / (pow(|h(j)|, 2)))
     * where j = 0 ... i-1
     *
     */
    template<typename T>
    utils::CMatrix<T> *gramSchmidtVectorBasis(const utils::CMatrix<T> &a,
                                             size_t startRow = 0,
                                             size_t startCol = 0,
                                             size_t rowSize = 0,
                                             size_t colSize = 0) {
        if (a.getRowSize() == 0) {
            throw std::logic_error("Row size == 0");
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
        utils::CArrayList<double> rowNormos = utils::CArrayList<double>(
                rowSize);

        size_t rowIndex = 0;
        for (size_t i = 0; i < rowSize; ++i) {
            const T *x = a.getRowArray(i + startRow) + startCol;
            double normH = normArray(x, colSize);
            if (normH != 0) {
                // добавляем и обрабатываем только если норма вектора != 0
                rowNormos.push(normH);
                if (rowIndex == 0) {
                    // начальное значение ортогонального вектора инициализуется
                    // первым вектором норма которого !=0
                    vectors->pushRow(x);
                } else {
                    // вычисление по предыдущим ортогональным векторам
                    utils::CArrayList<T> tempSum = utils::CArrayList<T>(
                            colSize);
                    for (size_t vectorIndex = 0;
                         vectorIndex < vectors->getRowSize(); ++vectorIndex) {
                        const T *h = vectors->getRowArray(vectorIndex);
                        double norm = rowNormos[vectorIndex];
                        double dist = distanceArrays(x, h, colSize);
                        double k = dist / (norm * norm);

                        std::cout << "k [" << rowIndex << "]: "<< k << std::endl;

                        utils::CArrayList<T> temp = utils::CArrayList<T>(
                                colSize);
                        temp.write(0, h, colSize);
                        temp.multiply(k);
                        if (vectorIndex == 0) {
                            tempSum.write(0, temp.getArray(), colSize); // init
                        } else {
                            tempSum.sum(temp); // sum
                        }
                    }

                    tempSum.reverseMinus(x);
                    std::cout << "tempSum" << std::endl;
                    tempSum.print();

                    vectors->pushRow(tempSum.getArray());
                }
                ++rowIndex;
            }
        }

        return vectors;
    }
}