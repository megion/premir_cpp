/**
 * Маркировка карты SOM
 */

#ifndef SRC_KOHONEN_LABELING_SOMLABELING_H
#define SRC_KOHONEN_LABELING_SOMLABELING_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/HashMapArray.h"
#include "utils/hash/HashEngine.h"
#include "models/models.h"
#include "utils/console_colors.h"

namespace kohonen {
    namespace labeling {

        struct LabelInfo {
            size_t count;
            double scaledCount;
        };

        struct LabelSummary {
            size_t minCount;
            size_t maxCount;
        };

        template<typename InRow, typename Label>
        class KeyLabelResolver {
        public:
            virtual Label &getKeyLabel(InRow &inDataRow) const = 0;
        };

        class IncreaseCounterUpdater : public utils::ValueUpdater<LabelInfo> {

            void update(LabelInfo &oldValue, const LabelInfo &newValue, const size_t valueSizeof) const {
                oldValue.count++;
            }

        };

        class SummaryUpdater : public utils::ValueUpdater<LabelSummary> {
            void update(LabelSummary &oldValue, const LabelSummary &newValue, const size_t valueSizeof) const {
                if (oldValue.minCount > newValue.minCount) {
                    oldValue.minCount = newValue.minCount;
                }
                if (oldValue.maxCount < newValue.maxCount) {
                    oldValue.maxCount = newValue.maxCount;
                }
            }
        };

        template<typename InRow, typename Label>
        class SomLabeling {
        public:
            typedef typename utils::Entry<Label, LabelInfo> InfoEntry;
            typedef typename utils::R3DMatrix<bool, bool, InfoEntry>::Row Row;
            typedef typename utils::R3DMatrix<bool, bool, InfoEntry>::Cell Cell;

            SomLabeling(size_t xdim, size_t ydim, utils::hash::HashEngine<Label> *hashEngine,
                        KeyLabelResolver<InRow, Label> *_keyResolver) :
                    winnerLabelsUpdater(IncreaseCounterUpdater()),
                    keyResolver(_keyResolver) {
                size_t nCount = xdim * ydim;
                winnerLabels = new utils::HashMapArray<Label, LabelInfo>(nCount, hashEngine);
                summaryLabels = new utils::HashMapArray<Label, LabelSummary>(1, hashEngine);
            }

            void addWinner(size_t winnerIndex, InRow &inDataRow) {
                LabelInfo info = {1, 0};
                winnerLabels->pushValue(winnerIndex, keyResolver->getKeyLabel(inDataRow), info, &winnerLabelsUpdater);
            }

            void scale() {
                SummaryUpdater summaryUpdater;
                // 1. collect statistics
                for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {
                    Row &row = (*winnerLabels->getMatrix())[r];
                    for (size_t c = 0; c < row.cellSize; ++c) {
                        Cell &cell = row[c];
                        for (size_t p = 0; p < cell.pointSize; ++p) {
                            InfoEntry &e = cell[p];
                            LabelSummary summary = {e.value.count, e.value.count};
                            summaryLabels->pushValue(0, e.key, summary, &summaryUpdater);
                        }
                    }
                }

                // 2. scale label count values
                for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {
                    Row &row = (*winnerLabels->getMatrix())[r];
                    for (size_t c = 0; c < row.cellSize; ++c) {
                        Cell &cell = row[c];
                        for (size_t p = 0; p < cell.pointSize; ++p) {
                            InfoEntry &e = cell[p];
                            LabelSummary *summary = summaryLabels->getValue(0, e.key);

                            e.value.scaledCount =
                                    (e.value.count - (*summary).minCount) / ((*summary).maxCount - (*summary).minCount);
                        }
                    }
                }
            }

            ~SomLabeling() {
                delete winnerLabels;
                delete summaryLabels;
            }

        private:
            utils::HashMapArray<Label, LabelInfo> *winnerLabels;
            utils::HashMapArray<Label, LabelSummary> *summaryLabels;
            KeyLabelResolver<InRow, Label> *keyResolver;
            IncreaseCounterUpdater winnerLabelsUpdater;

        };
    }
}


#endif
