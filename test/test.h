#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <stdio.h>
#include <assert.h>

#include "utils/console_colors.h"

/*
 * Test the given `fn`.
 */

#define mytest(fn) \
  printf("    " GREEN "✓ %s" RESET "\n", #fn); \
  test_##fn();

/*
 * Test suite title.
 */

#define suite(title) \
  printf("\n  " CYAN "%s" RESET "\n", title)

/*
 * Report sizeof.
 */

#define print_size(type) \
  printf("\n  \e[90m%s: %ld bytes\e[0m\n", #type, sizeof(type));

#define assert_range(val, middle, offset) \
  assert( (middle-offset < val) && (val < middle+offset));

namespace test {
	namespace common {

		struct CallInfo {
			size_t defaultConstructorCallCount;
			size_t copyConstructorCallCount;
			size_t moveConstructorCallCount;

			size_t copyOperatorCallCount;
			size_t moveOperatorCallCount;

			size_t destructorCallCount;
			size_t destructorAndClearResourcesCallCount;

			CallInfo():
				defaultConstructorCallCount(0),
				copyConstructorCallCount(0),
				moveConstructorCallCount(0),
				copyOperatorCallCount(0),
				moveOperatorCallCount(0),
				destructorCallCount(0),
				destructorAndClearResourcesCallCount(0) {

			}

			CallInfo(size_t _defaultConstructorCallCount,
					size_t _copyConstructorCallCount,
					size_t _moveConstructorCallCount,
					size_t _copyOperatorCallCount,
					size_t _moveOperatorCallCount,
					size_t _destructorCallCount,
					size_t _destructorAndClearResourcesCallCount):
				defaultConstructorCallCount(_defaultConstructorCallCount),
				copyConstructorCallCount(_copyConstructorCallCount),
				moveConstructorCallCount(_moveConstructorCallCount),
				copyOperatorCallCount(_copyOperatorCallCount),
				moveOperatorCallCount(_moveOperatorCallCount),
				destructorCallCount(_destructorCallCount),
				destructorAndClearResourcesCallCount(_destructorAndClearResourcesCallCount) {

			}

			void reset() {
				defaultConstructorCallCount = 0;
				copyConstructorCallCount = 0;
				moveConstructorCallCount = 0;
				copyOperatorCallCount = 0;
				moveOperatorCallCount = 0;
				destructorCallCount = 0;
				destructorAndClearResourcesCallCount = 0;
			}
		};
	}
}

#endif /* TEST_TEST_H_ */
