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


#endif /* TEST_TEST_H_ */
