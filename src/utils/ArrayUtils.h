/**
 * Утилиты для векторов
 */

#ifndef SRC_UTILS_VECTORUTILS_H
#define SRC_UTILS_VECTORUTILS_H

#include <math.h>

#include "utils/SMatrix.h"
#include "utils/console_colors.h"

namespace utils {

    template<typename T, typename R>
    class ArrayUtils {

    public:
        ArrayUtils() {
        }

        ~ArrayUtils() {
        }

        static void normalization(const T* a, size_t sizeArray) {
            T sum=0;
            for (size_t i=0; i<sizeArray; i++) {
                sum+=a[i]*a[i];
            }
            sum=sqrt(sum);
            for (size_t i=0; i<n; ++i) {
                a[i] /= sum;
            }
        }

//        utils::SMatrix<R>* findEigenVectors(utils::SMatrix<T>& inMatrix) {
//        }


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
