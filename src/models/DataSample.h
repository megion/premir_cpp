#ifndef SRC_MODELS_DATASAMPLE_H
#define SRC_MODELS_DATASAMPLE_H

#include <cstdio>

namespace models {
    /**
     * Хранить прочитанное значение
     */
    template<typename T>
    struct DataSample {
        T value; // значение
        bool skipped; // true значение пропущено или неопределенно
    };

    /**
     * Хранит некоторую информацию о нейроне.
     * Например skipped используется чтобы исключить нейрон из подсчета
     * в некоторых алгоритмах.
     */
    struct NeuronInfo {
        bool skipped; // true значение необходимо пропустить при подсчетах

        bool operator==(const NeuronInfo &other) const {
            if (other.skipped != skipped) {
                return false;
            }
            return true;
        }

        bool operator!=(const NeuronInfo &other) const {
            return !((*this) == other);
        }
    };
}


#endif
