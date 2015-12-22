#ifndef SRC_ML_GRAMSCHMIDTBASIS_H
#define SRC_ML_GRAMSCHMIDTBASIS_H


#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "utils/CArrayList.h"
#include "utils/CMatrix.h"
#include "utils/matrix_utils.h"

namespace ml {

    class GramSchmidtBasis {
    public:
        GramSchmidtBasis(size_t _inVectorSize) :
                inVectorSize(_inVectorSize) {
            outVectors = new utils::CMatrix<double>(0, inVectorSize);
            squaredNormsCache = new utils::CArrayList<double>();
        }

        ~GramSchmidtBasis() {
            delete outVectors;
            delete squaredNormsCache;
        }

        utils::CMatrix<double>* getOutVectors() {
            return outVectors;
        }

        bool pushInVector(double *inArray);

    private:
        size_t inVectorSize; // размер входного массива
        utils::CMatrix<double> *outVectors; // выходной массив векторов
        utils::CArrayList<double> *squaredNormsCache; // массив посчитанных норм

    };

}


#endif
