//
// Created by ilya on 4/11/16.
//

#ifndef SRC_ML_DECISIONTREE_SPLITRESULT_H
#define SRC_ML_DECISIONTREE_SPLITRESULT_H

#include "utils/CArrayList.h"

namespace ml {
	namespace decisiontree {

		template<typename In>
		class SplitResult {

		public:
			SplitResult() {
				matchResults = new utils::CArrayList<In>();
				notMatchResults = new utils::CArrayList<In>();
			}

			~SplitResult() {
				delete matchResults;
				delete notMatchResults;
			}

			utils::CArrayList<In> *getMatchResults() {
				return matchResults;
			}

			utils::CArrayList<In> *getNotMatchResults() {
				return notMatchResults;
			}

		private:
			utils::CArrayList<In> *matchResults;
			utils::CArrayList<In> *notMatchResults;
		};

	}
}
#endif

