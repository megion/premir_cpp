#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <stdio.h>
#include <assert.h>

/*
 * Test the given `fn`.
 */

#define test(fn) \
  printf("    \e[92m✓ \e[90m%s\e[0m\n", #fn); \
  test_##fn();

/*
 * Test suite title.
 */

#define suite(title) \
  printf("\n  \e[36m%s\e[0m\n", title)

/*
 * Report sizeof.
 */

#define print_size(type) \
  printf("\n  \e[90m%s: %ld bytes\e[0m\n", #type, sizeof(type));

#define assert_range(val, middle, offset) \
  assert( (middle-offset < val) && (val < middle+offset));


#endif /* TEST_TEST_H_ */
