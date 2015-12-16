/*
 * test.h
 *
 *  Created on: Sep 15, 2015
 *      Author: ilya
 */

#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <stdio.h>
#include <assert.h>
//#include <cstdlib>

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


#endif /* TEST_TEST_H_ */
