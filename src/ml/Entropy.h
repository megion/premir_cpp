//
// Created by ilya on 4/8/16.
//

#ifndef SRC_ML_ENTROPY_H
#define SRC_ML_ENTROPY_H

#include <cstdio>
#include <cmath>

#include "utils/HashMap.h"
#include "utils/hash/HashEngine.h"

namespace ml {

    struct EntropyValueInfo {
        size_t count;

        friend std::ostream &operator<<(std::ostream &os, const EntropyValueInfo &val) {
            os << "count " << val.count;
            return os;
        }
    };

    class CounterUpdater : public utils::ValueUpdater<EntropyValueInfo> {

        void update(EntropyValueInfo &oldValue, const EntropyValueInfo &newValue, const size_t valueSizeof) const {
            oldValue.count++;
        }

    };

    /**
     * Calculate combinatorial entropy
     */
    template<typename V>
    class Entropy {

    public:
        typedef typename utils::Entry<V, EntropyValueInfo> VEntry;
        typedef typename utils::RDMatrix<bool, VEntry>::Row VRow;

        Entropy(utils::hash::HashEngine<V> *hashEngine) :
                counterUpdater(CounterUpdater()), totalCounter(0) {
            valueCounters = new utils::HashMap<V, EntropyValueInfo>(hashEngine);
        }

        ~Entropy() {
            delete valueCounters;
        }

        void addValue(const V &val) {
            EntropyValueInfo info = {1};
            valueCounters->pushValue(val, info, &counterUpdater);
            totalCounter++;
        }

        /**
         * Вычисление комбинаторной энтропии
         */
        double calculateEntropy() {
            double prod = 1;
            for (size_t r = 0; r < valueCounters->getMatrix()->getRowSize(); ++r) {
                VRow &row = (*valueCounters->getMatrix())[r];
                for (size_t p = 0; p < row.pointSize; ++p) {
                    VEntry &e = row[p];
                    double f = factorial(e.value.count);
                    prod = prod * f;
                }
            }

            // мультиномиальний коэффициент - количество перестановок
            double w = factorial(totalCounter)/prod;
            return std::log2(w)/(double)totalCounter;
        }

    private:
        utils::HashMap<V, EntropyValueInfo> *valueCounters;
        size_t totalCounter;
        CounterUpdater counterUpdater;

        double factorial(const size_t& n) {
            double result = 1.0;
            for (size_t i = 2; i <= n; i++) {
                result *= i;
            }
            return result;
        }

    };
}

#endif
