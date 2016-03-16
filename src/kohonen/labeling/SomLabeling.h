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

        class IncreaseCounterUpdater : public utils::ValueUpdater<models::LabelInfo> {

            void update(models::LabelInfo &oldValue, const models::LabelInfo &newValue,
                        const size_t valueSizeof) const {
                oldValue.count++;
            }

        };

        class SummaryUpdater : public utils::ValueUpdater<models::LabelSummary> {
            void update(models::LabelSummary &oldValue, const models::LabelSummary &newValue,
                        const size_t valueSizeof) const {
                if (oldValue.minCount > newValue.minCount) {
                    oldValue.minCount = newValue.minCount;
                }
                if (oldValue.maxCount < newValue.maxCount) {
                    oldValue.maxCount = newValue.maxCount;
                }
            }
        };

        template<typename Label>
        class SomLabeling {
        public:
            typedef typename utils::Entry<Label, models::LabelInfo> InfoEntry;
            typedef typename utils::R3DMatrix<bool, bool, InfoEntry>::Row InfoRow;
            typedef typename utils::R3DMatrix<bool, bool, InfoEntry>::Cell InfoCell;

            SomLabeling(size_t xdim, size_t ydim, utils::hash::HashEngine<Label> *hashEngine) :
                    winnerLabelsUpdater(IncreaseCounterUpdater()) {
                size_t nCount = xdim * ydim;
                winnerLabels = new utils::HashMapArray<Label, models::LabelInfo>(nCount, hashEngine);
                summaryLabels = new utils::HashMapArray<Label, models::LabelSummary>(1, hashEngine);
            }

            void addWinner(size_t winnerIndex, Label &label) {
                models::LabelInfo info = {1, 0};
                winnerLabels->pushValue(winnerIndex, label, info, &winnerLabelsUpdater);
            }

            /**
             * Clean winner labels hash maps
             */
            void cleanWinnerLabels() {
                winnerLabels->getMatrix()->removeAllRowCells();
            }

            void collectSummary() {
                // 0. clean prev summary
                summaryLabels->getMatrix()->removeRowPoints(0);

                SummaryUpdater summaryUpdater;
                // 1. collect statistics
                for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {
                    InfoRow &row = (*winnerLabels->getMatrix())[r];
                    for (size_t c = 0; c < row.cellSize; ++c) {
                        InfoCell &cell = row[c];
                        for (size_t p = 0; p < cell.pointSize; ++p) {
                            InfoEntry &e = cell[p];
                            models::LabelSummary summary = {e.value.count, e.value.count};
                            summaryLabels->pushValue(0, e.key, summary, &summaryUpdater);
                        }
                    }
                }
//                summaryLabels->getMatrix()->print();

                // 2. scale label count values
                for (size_t r = 0; r < winnerLabels->getMatrix()->getRowSize(); ++r) {
                    InfoRow &row = (*winnerLabels->getMatrix())[r];
                    for (size_t c = 0; c < row.cellSize; ++c) {
                        InfoCell &cell = row[c];
                        for (size_t p = 0; p < cell.pointSize; ++p) {
                            InfoEntry &e = cell[p];
                            models::LabelSummary *summary = summaryLabels->getValue(0, e.key);
                            if ((*summary).maxCount==0 || (*summary).maxCount==1) {
                                e.value.scaledCount = 0;
                            } else {
                                e.value.scaledCount = ((double)e.value.count) / (*summary).maxCount;
                            }
                        }
                    }
                }

//                winnerLabels->getMatrix()->print(false);
            }

            ~SomLabeling() {
                delete winnerLabels;
                delete summaryLabels;
            }

            utils::HashMapArray<Label, models::LabelInfo> *getWinnerLabels() const {
                return winnerLabels;
            };

            utils::HashMapArray<Label, models::LabelSummary> *getSummaryLabels() const {
                return summaryLabels;
            };

        private:
            utils::HashMapArray<Label, models::LabelInfo> *winnerLabels;
            utils::HashMapArray<Label, models::LabelSummary> *summaryLabels;
            IncreaseCounterUpdater winnerLabelsUpdater;

        };
    }
}


#endif
