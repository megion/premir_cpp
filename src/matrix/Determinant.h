/**
 * Calculate matrix determinant
 */

#ifndef SRC_MATRIX_DETERMINANT_H
#define SRC_MATRIX_DETERMINANT_H

#include <math.h>

#include "utils/SMatrix.h"
#include "utils/console_colors.h"

namespace matrix {

    template<typename T, typename R>
    class Determinant {

    public:
        Determinant() {
        }

        /**
         * Улучшенный алгоритм нахождения детерминанта матрицы по алгоритму
         * Гаусса.
         * Алгоритм Гаусса:
         * 1. Преобразование исходной матрицы к треугольной.
         * 2. Нахождение детерминанта как произведение диаголнальных элементов
         * треугольной матрицы.
         *
         * Алгоритм преобразования исходной матрицы к треугольной
         *   // for j = 0 ... N - 2
         *   //     find row k (k = j, ... N-1) where |a(k,j)| is max
         *   //     for i = j + 1 ... N - 1
         *   //         a(i) = a (i) - (a(i,j)/a(j,j)) * a(j)
         *
         * Нахождение детерминанта
         *   // calculate determinant as
         *   // d = a(0,0)
         *   // for i = 1 .. N - 1
         *   //     d = d * a(i,i)
         *
         * Улучшение алгоритма состоит в том чтобы найти строку k c максимальным
         * элементом ajk (увеличение знаменателя, уменьшает ошибку).
         */
        R gaussDeterminant(utils::SMatrix<T>& matrix) {
            size_t matrixSize = matrix.getColSize();
            size_t swapCount = 0;
            R determinant = 1;

            // make triangular matrix by gauss algorithm
            // for j = 0 ... N - 2
            //     find row k (k = j, ... N-1) where |a(k,j)| is max
            //     for i = j + 1 ... N - 1
            //         a(i) = a (i) - (a(i,j)/a(j,j)) * a(j)
            for (size_t j = 0; j<(matrixSize-1); ++j) {
                // find row k (k = j, ... N-1) where |a(k,j)| is max
                T akjMax = fabs(matrix(j,j)); // init first as |a(j,j)|
                size_t maxK = j;
                for (size_t k = j + 1; k<matrixSize; ++k) {
                    const T& akj = fabs(matrix(k,j));
                    if (akj > akjMax) {
                        akjMax = akj;
                        maxK = k;
                    }
                }

                if (maxK > j) {
                    // swap row k and j
                    matrix.swapRows(maxK, j);
                    ++swapCount;
                } else if (akjMax == T(0)) {
                    // skip iteration because all j column elements is 0
                    // TODO: необходимо переместится на столбец правее
                    // и понизить ранг матрицы
                    danger_text("All column elements == 0 (MAX|a(k,j)|==0)");
                    return akjMax;
                }

                const T& ajj = matrix(j,j);
                determinant = determinant * ajj;

                for (size_t i=j+1; i<matrixSize; ++i) {
                    R r = matrix(i,j)/ajj;
                    T* aiVec = matrix[i];
                    T* ajVec = matrix[j];
                    matrix(i,j) = 0; // set 0
                    // aiVec = ajVec - r * ajVec
                    for (size_t n = j + 1; n<matrixSize; ++n) {
                        aiVec[n] = aiVec[n] - r * ajVec[n];
                    }
                }
            }

            determinant = determinant * matrix(matrixSize-1, matrixSize-1);
            if(swapCount%2==0) {
                return determinant;
            } else {
                return -determinant;
            }
        }

        /**
         * Реализация алгоритма Барейса для нахождения детерминанта матрицы.
         * Особенностью Алгоритм Барейса является то, что если элементы матрицы
         * целые числа, то и результат деления также целое число поэтому и
         * детерминант матрицы целое число. Следовательно для целых чисел этот
         * алгоритм не приводит к потере точности.
         *
         * 1. Преобразование к треугольной матрице Барейса
         * // a(-1, -1) = 1
         * // for j = 0 ... N - 2
         * //     for i = j + 1 ... N - 1
         * //         a(i) = (a(j,j) *  a (i)  - a(i,j) * a(j))) / a(j-1,j-1)
         *
         * 2. Нижний правый элемент треугольный матрицы будет равен детерминанту
         *
         * Примечание:
         * Несмотря на то, что алгоритм Барейса позволяет точно и быстро
         * посчитать определитель матрицы, он мало пригоден для решения систем
         * линейных уравнений большой размерности. Дело в том, что по мере
         * приведения матрицы к треугольному виду методом Барейса, получаемые
         * числа по порядку величины равны определителю обработанной на данный
         * момент части матрицы. Может так оказаться, что определители
         * настолько велики, что не умещаются в используемый тип данных.
         * Это означает, что определитель такой матрицы не может быть
         * посчитан с использованием имеющегося типа данных, но это совершенно
         * не означает, что такую систему уравнений нельзя решить. В таких
         * случаях лучше использовать алгоритм Гаусса с перестановкой строк.
         */
        T bareissDeterminant(utils::SMatrix<T>& matrix) {
            size_t matrixSize = matrix.getColSize();
            size_t swapCount = 0;
            T ajjPrev = 1;

            for (size_t j = 0; j<(matrixSize-1); ++j) {
                // повышение точно путем перестановки строк
                // find row k (k = j, ... N-1) where |a(k,j)| is max
                T akjMax = fabs(matrix(j,j)); // init first as |a(j,j)|
                size_t maxK = j;
                for (size_t k = j + 1; k<matrixSize; ++k) {
                    const T& akj = fabs(matrix(k,j));
                    if (akj > akjMax) {
                        akjMax = akj;
                        maxK = k;
                    }
                }
                if (maxK > j) {
                    // swap row k and j
                    matrix.swapRows(maxK, j);
                    ++swapCount;
                } else if (akjMax == T(0)) {
                    // skip iteration because all j column elements is 0
                    // TODO: необходимо переместится на столбец правее
                    // и понизить ранг матрицы
                    danger_text("All column elements == 0 (MAX|a(k,j)|==0)");
                    return akjMax;
                }

                const T& ajj = matrix(j,j);
                for (size_t i=j+1; i<matrixSize; ++i) {
                    T* aiVec = matrix[i];
                    T* ajVec = matrix[j];
                    T aij = matrix(i,j); // save copy
                    matrix(i,j) = 0; // set 0
                    for (size_t n = j + 1; n<matrixSize; ++n) {
                        aiVec[n] = (ajj * aiVec[n] - aij * ajVec[n])/ajjPrev;
                    }
                }
                ajjPrev = ajj; // save copy
            }

            if(swapCount%2==0) {
                return matrix(matrixSize-1, matrixSize-1);
            } else {
                return -matrix(matrixSize-1, matrixSize-1);
            }
        }

    };
}

#endif
