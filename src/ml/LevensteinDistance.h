/**
 * Calculate weighted Levenstein distance WLD.
 * WLD = min{p*a(i) + q*b(i) + r*c(i)}
 */

#ifndef SRC_ML_LEVENSTEINDISTANCE_H
#define SRC_ML_LEVENSTEINDISTANCE_H

#include <math.h>
#include <wchar.h>
#include <wctype.h>

#include "utils/SMatrix.h"
#include "utils/console_colors.h"

namespace ml {

    template<typename T>
    class LevensteinDistance {

    public:

        LevensteinDistance() {
        }

        ~LevensteinDistance() {
        }

        /**
         *
         */
        T wld(const wchar_t *s1, const wchar_t *s2, size_t s1len,
              size_t s2len) {
            utils::SMatrix<T> matrix(s1len + 1, s2len + 1);

            // init matrix
            matrix(0, 0) = 0;
            for (size_t i = 1; i <= s1len; ++i) {
                matrix(i, 0) = matrix(i - 1, 0) + 1;
            }
            for (size_t j = 1; j <= s2len; ++j) {
                matrix(0, j) = matrix(0, j - 1) + 1;
            }

            for (size_t i = 1; i <= s1len; ++i) {
                for (size_t j = 1; j <= s2len; ++j) {
                    int ps1s2 = (s1[i - 1] == s2[j - 1] ? 0 : 1);
                    T m1 = matrix(i - 1, j - 1) + ps1s2;
                    T m2 = matrix(i, j - 1) + 1;
                    T m3 = matrix(i - 1, j) + 1;
                    matrix(i, j) = min3(m1, m2, m3);
                }
            }

//            matrix.print();

            return matrix(s1len, s2len);
        }

        T wldOptimazed(const wchar_t *s1, const wchar_t *s2, size_t s1len,
                       size_t s2len) {
            T column[s1len + 1];
            // init
            for (size_t i = 1; i <= s1len; ++i) {
                column[i] = i;
            }
            for (size_t j = 1; j <= s2len; ++j) {
                column[0] = j;
                T oldDist;
                T lastDist = j - 1;
                for (size_t i = 1; i <= s1len; ++i) {
                    oldDist = column[i];
                    T m1 = column[i] + 1;
                    T m2 = column[i - 1] + 1;
                    T m3 = lastDist + (s1[i - 1] == s2[j - 1] ? 0 : 1);
                    column[i] = min3(m1, m2, m3);
                    lastDist = oldDist;
                }
            }
            return (column[s1len]);
        }

    private:
        T &min3(T &a, T &b, T &c) const {
            return ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b)
                                                                     : (c)));
        }

    };
}

#endif
