/**
 * Утилиты для векторов
 */

#ifndef SRC_UTILS_VECTORUTILS_H
#define SRC_UTILS_VECTORUTILS_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "sort/HeapSort.h"

namespace utils {

    template<typename InA, typename InB, typename Out>
    class ArrayUtils {

    public:
        ArrayUtils() {
        }

        ~ArrayUtils() {
        }

        static void normalization(InA *a, size_t sizeArray) {
            InA sum = 0;
            for (size_t i = 0; i < sizeArray; i++) {
                sum += a[i] * a[i];
            }
            sum = sqrt(sum);
            for (size_t i = 0; i < sizeArray; ++i) {
                a[i] /= sum;
            }
        }

        /**
         * Вычисление квадрата евклидовой нормы массива
         * result = sum(pow(a(i), 2)) where i = 0...N-1
         */
        static Out euclideanSquaredNorm(const InA *a, size_t sizeArray) {
            Out res = 0;
            for (size_t i = 0; i < sizeArray; ++i) {
                const InA &ai = *(a + i);
                res += ai * ai;
            }
            return res;
        }

        /**
         * Вычисление медианы массива:
         *
         * 1) сортировка массива - результат сортировки будет записан в
         * исходный массив
         * 2) если количество элементов массива четное - то медиана
         * это середина между двумя средними элементами
         * если нечетное то это средний элемент
         *
         */
        static Out median(InA *a, size_t sizeArray) {
            sort::HeapSort<InA> hsort;
            hsort.sort(a, sizeArray);

            if (sizeArray%2==0) {
                // четное
                size_t m = sizeArray/2 - 1;
                return (Out)(a[m] + a[m+1]) / 2;
            } else {
                // нечетное
                size_t m = sizeArray/2;
                return (Out)a[m];
            }
        }

        /**
         * Вычисление среднего значения массива: сумма всех элементов деленных
         * на их число
         *
         */
        static Out average(InA *a, size_t sizeArray) {
            Out sum = 0;
            for (size_t i=0; i<sizeArray; ++i) {
                sum+=a[i];
            }
            return (Out)sum/sizeArray;
        }

        /**
         * Вычисление евклидовой нормы массива
         * result = pow(sum(pow(a(i), 2)), 0.5) where i = 0...N-1
         */
        static Out euclideanNorm(const InA *a, size_t sizeArray) {
            return std::pow(ArrayUtils<InA, InB, Out>::
                    euclideanSquaredNorm(a, sizeArray), 0.5);
        }

        /**
         * Скаларное произведение массивов A и B.
         * result = sum(a(i)*b(i)) where i = 0...N-1
         */
        static Out scalarMultiplication(
                const InA *a, const InB *b, size_t sizeArray) {
            Out res = 0;
            for (size_t i = 0; i < sizeArray; ++i) {
                res = res + a[i] * b[i];
            }
            return res;
        }

        // a = b - a
        static void reverseMinus(InA *a, const InB *b, size_t sizeArray) {
            for (size_t i = 0; i < sizeArray; ++i) {
                a[i] = b[i] - a[i];
            }
        }

        /**
         * Евклидовое расстояние массивов A и B.
         * result = sum( (a(i)-b(i))*(a(i)-b(i)) ) where i = 0...N-1
         */
        static Out euclideanDistance(const InA *a, const InB *b,
                                     size_t sizeArray) {
            Out res = 0;
            Out diff = 0;
            for (size_t i = 0; i < sizeArray; ++i) {
                diff = a[i] - b[i];
                res = res + diff * diff;
            }
            return std::sqrt(res);
        }



    };
}

#endif
