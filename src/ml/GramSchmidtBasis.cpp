#include "GramSchmidtBasis.h"

namespace ml {

    bool GramSchmidtBasis::pushInVector(double *inArray) {
        if (outVectors->getRowSize() == 0) {
            // пустой поэтому вычислим квадрат нормы входного массива
            double inNormSquared = utils::euclideanSquaredNorm(inArray,
                                                               inVectorSize);
            // if != 0 then init first out value
            if (inNormSquared != 0) {
                outVectors->pushRow(inArray);
                squaredNormsCache->push(inNormSquared);
                return true;
            }
        } else {
            // calculate sum by previous calculated orthogonal vectors
            utils::CArrayList<double> tempSum = utils::CArrayList<double>(
                    inVectorSize);
            for (size_t i = 0; i < outVectors->getRowSize(); ++i) {
                double *h = outVectors->getRowArray(i);
                double dist = utils::multiplyArrays(inArray, h, inVectorSize);
                double squaredNorm = (*squaredNormsCache)[i];
                double k = dist / squaredNorm;

                utils::CArrayList<double> temp = utils::CArrayList<double>(
                        inVectorSize);
                temp.write(0, h, inVectorSize);
                temp.multiply(k);

                if (i == 0) {
                    tempSum.write(0, temp.getArray(), inVectorSize); // init
                } else {
                    tempSum.sum(temp); // sum
                }
            }

            tempSum.reverseMinus(inArray); // inArray - tempSum

            double squaredLastNorm = utils::euclideanSquaredNorm(
                    tempSum.getArray(),
                    inVectorSize);
            if (squaredLastNorm != 0) {
                outVectors->pushRow(tempSum.getArray());
                squaredNormsCache->push(squaredLastNorm);
                return true;
            }
        }

        return false;
    }

}
