#ifndef TEST_CACHE_DIR_CACHE_TEST_H
#define TEST_CACHE_DIR_CACHE_TEST_H

#include <cerrno>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "test.h"

#include "cache/ReadCache.h"

namespace test {
namespace cache {

void read_cache_test();
}
} // namespace test

#endif
