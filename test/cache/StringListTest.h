#ifndef TEST_CACHE_STRING_LIST_H
#define TEST_CACHE_STRING_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "test.h"

#include "cache/StringList.h"
#include "cache/DirCache.h"
#include "cache/strbuf.h"

namespace test {
    namespace cache_string_list {

		void stringList_test();

    }
}

#endif

