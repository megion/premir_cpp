#include "MlUtils.h"

namespace ml {

    double pearsonCorrelation(const models::DataSample *a, const models::DataSample *b,
                                              size_t sizeArray) {
        size_t countA = 0, countB = 0;
        double sumA = 0, sumB = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            if (!a[i].skipped) {
                countA++;
                sumA += a[i].value;
            }
            if (!b[i].skipped) {
                countB++;
                sumB += b[i].value;
            }
        }

        double averA = sumA / countA;
        double averB = sumB / countB;

        double covAB = 0;
        double sA = 0, sB = 0;
        for (size_t i = 0; i < sizeArray; ++i) {
            if (a[i].skipped || b[i].skipped) {
                continue;
            }

            covAB += (a[i].value - averA) * (b[i].value - averB);
            double dA = (a[i].value - averA);
            sA += dA * dA;
            double dB = (b[i].value - averB);
            sB += dB * dB;
        }

        return covAB / std::sqrt(sA * sB);
    }
}

