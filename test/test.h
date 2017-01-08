#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <stdio.h>
#include <assert.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

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
