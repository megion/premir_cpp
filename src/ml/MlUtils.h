/**
 *
 */
#ifndef SRC_ML_MLUTILS_H
#define SRC_ML_MLUTILS_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "models/models.h"

namespace ml {

    /**
     * Коэффициент корреляции Пирсона массивов A и B.
     */
    double pearsonCorrelation(const models::DataSample *a, const models::DataSample *b, size_t sizeArray);
}

#endif
