#ifndef SRC_ML_DECISIONTREE_SPLITMATCHER_H
#define SRC_ML_DECISIONTREE_SPLITMATCHER_H

#include <cstdio>
#include <cmath>

namespace ml {
	namespace decisiontree {

        class SplitMatcher {
        public:

        	SplitMatcher() {
            }

            virtual void adaptation() = 0;

        protected:

        };
    }
}

#endif
