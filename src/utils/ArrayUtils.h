/**
 * Утилиты для векторов
 */

#ifndef SRC_UTILS_VECTORUTILS_H
#define SRC_UTILS_VECTORUTILS_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

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
                res = res + ai * ai;
            }
            return res;
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


//        double normArray(const double *a, size_t sizeArray) {
//            double res = 0;
//            for (size_t i = 0; i < sizeArray; ++i) {
//                const double &ai = *(a + i);
//                res = res + std::abs(ai);
//            }
//            return res;
//        }


//        void normalize(float *v, int n)
//        {
//            float sum=0.0;
//            int j;
//
//            for (j=0; j<n; j++) sum+=v[j]*v[j];
//            sum=sqrt(sum);
//            for (j=0; j<n; j++) v[j]/=sum;
//        }
//
//
//        float dotprod(float *v, float *w, int n)
//        {
//            float sum=0.0;
//            int j;
//
//            for (j=0; j<n; j++) sum+=v[j]*w[j];
//            return (sum);
//        }

    };
}

#endif
