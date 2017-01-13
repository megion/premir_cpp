#ifndef TEST_UTILS_HASH_MAP_TEST_H
#define TEST_UTILS_HASH_MAP_TEST_H

#include <iostream>
#include <cstdio>

#include "test.h"
#include "utils/HashMap.h"
#include "utils/hash/HashEngine.h"
#include "utils/hash/CharHash.h"

namespace test {
	namespace utils_hash_map {

		void hashMap_test();

		struct MyValue {
			int count;
			int value2;
		};

		struct MyStringKey {
			char label[256];
			size_t length;
			int count2;
			void setLabel(const char* val) {
				size_t len = std::strlen(val);
				std::memcpy(label, val, len);
				label[len] = '\0';
				length = len;
			}

			// compare function for hash map
			bool operator==(const MyStringKey &other) const {
				if (length != other.length) {
					return false;
				}
				return std::strncmp(label, other.label, length) == 0;
			}
			bool operator!=(const MyStringKey &other) const {
				return !((*this) == other);
			}
		};

		class MyStringKeyHash: public utils::hash::HashEngine<MyStringKey> {
		public:
			MyStringKeyHash() {
			}
		protected:
			size_t hashCode(const MyStringKey& value) const {
				return utils::hash::HashEngine<MyStringKey>::charsHash(value.label, value.length);
			}
		};

		class MyValueUpdater: public utils::ValueUpdater<MyValue> {

			void update(MyValue& oldValue, const MyValue& newValue, const size_t valueSizeof) const {
				oldValue.count++;
			}

		};

		class MyKeyUpdater: public utils::ValueUpdater<MyStringKey> {

			void update(MyStringKey& oldValue, const MyStringKey& newValue, const size_t valueSizeof) const {
				oldValue.count2++;
			}

		};
	}
}

#endif

