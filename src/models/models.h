#ifndef SRC_MODELS_MODELS_H
#define SRC_MODELS_MODELS_H

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
     * Хранит некоторую статистику для одной колонки. Статистика собирается только по допустимым значениям.
     */
    template<typename T>
    struct ColSummary {
        T min; // минимальное допустимое значение
        T max; // максимальное (допустимое) значение в колонке
        double sum; // сумма допустимых значений в колонке
        double average; // среднее значение sum/count
        size_t count; // число допустимых значенией в колонке (не все значения являются допустимыми)
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
