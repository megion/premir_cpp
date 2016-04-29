#ifndef SRC_ML_DECISIONTREE_SPLITMATCHER_H
#define SRC_ML_DECISIONTREE_SPLITMATCHER_H

#include <cstdio>
#include <cmath>

namespace ml {
	namespace decisiontree {

		template<typename In>
        class SplitMatcher {
        public:

        	SplitMatcher() {
            }

            virtual bool match(In& itemA, In& itemB) = 0;

        protected:

        };
    }
}

#endif
