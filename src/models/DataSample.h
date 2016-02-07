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
}


#endif
