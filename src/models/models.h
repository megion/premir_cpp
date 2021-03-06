#ifndef SRC_MODELS_MODELS_H
#define SRC_MODELS_MODELS_H

#include <cstdio>
#include <iostream>

namespace models {
    /**
     * Хранить прочитанное значение
     */
    struct DataSample {
        double value; // значение
        bool skipped; // true значение пропущено или неопределенно
    };

    /**
     * Хранит некоторую статистику для одной колонки. Статистика собирается только по допустимым значениям.
     */
    struct ColSummary {
        double min; // минимальное допустимое значение
        double max; // максимальное (допустимое) значение в колонке
        double sum; // сумма допустимых значений в колонке
        double average; // среднее значение sum/count
        double scaledAverage; // нормализованное среднее значение
        size_t count; // число допустимых значенией в колонке (не все значения являются допустимыми)

        const double &getAverage(const bool &isScale) const {
            return isScale ? scaledAverage : average;
        }

        bool isSkip() const {
            return (count==0) || (max == min);
        }

        bool operator==(const ColSummary &other) const {
            if ((other.min != min) || (other.max != max) || (other.sum != sum) || (other.average != average) ||
                (other.scaledAverage != scaledAverage) || (other.count != count)) {
                return false;
            }

            return true;
        }

        bool operator!=(const ColSummary &other) const {
            return !((*this) == other);
        }

        friend std::ostream &operator<<(std::ostream &os, const ColSummary &val) {
            os << std::endl << "min " << val.min << ", max " << val.max << ", sum " << val.sum << ", average " <<
            val.average << ", scaledAverage " << val.scaledAverage << ", count " << val.count;
            return os;
        }
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

    struct LabelInfo {
        size_t count;
        double scaledCount;

        friend std::ostream &operator<<(std::ostream &os, const LabelInfo &val) {
            os << "count " << val.count << ", scaledCount " << val.scaledCount;
            return os;
        }
    };

    struct LabelSummary {
        size_t minCount;
        size_t maxCount;

        friend std::ostream &operator<<(std::ostream &os, const LabelSummary &val) {
            os << "minCount " << val.minCount << ", maxCount " << val.maxCount;
            return os;
        }
    };


}


#endif
