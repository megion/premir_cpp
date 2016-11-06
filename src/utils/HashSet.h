/**
 * Hash set
 */

#ifndef SRC_UTILS_HASHSET_H
#define SRC_UTILS_HASHSET_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include "utils/RDMatrix.h"
#include "utils/hash/HashEngine.h"
#include "HashMapArray.h"

namespace utils {

	template<typename K>
	class HashSet {
	public:

		typedef typename utils::RDMatrix<bool, K>::Row Row;

		HashSet(hash::HashEngine<K> *_hashEngine) :
				hashEngine(_hashEngine), keySizeof(sizeof(K)) {
			matrix = new utils::RDMatrix<bool, K>();
		}

		HashSet<K>(const HashSet<K> &) = delete; // disable copy constructor
		HashSet<K> &operator=(const HashSet<K> &) = delete; // assign operator
		HashSet<K> &operator=(HashSet<K> &&) = delete; // replacement operator

		~HashSet() {
			delete matrix;
			matrix = nullptr;
		}

		bool operator==(const HashSet<K> &other) const {
			if ((*other.getMatrix()) != (*matrix)) {
				return false;
			}
			return true;
		}

		bool operator!=(const HashSet<K> &other) const {
			return !((*this) == other);
		}

		utils::RDMatrix<bool, K> *getMatrix() const {
			return matrix;
		}

		bool push(const K &value, const ValueUpdater<K> *updater = nullptr) {
			size_t rowIndex = hashEngine->hashCode(value);

			if (rowIndex < matrix->getRowSize()) {
				// пройти по точкам с одинаковым хэшкодом (значения коллизий)
				Row &row = (*matrix)[rowIndex];
				for (size_t i = 0; i < row.pointSize; ++i) {
					if (row[i] == value) {
						// найдено значение с уже имеющимся ключом - exit
						if (updater) {
							// custom update function
							updater->update(row[i], value, keySizeof);
						}
						return false;
					}
				}
				// key not found - add entry to end
				matrix->writeToEndRow(rowIndex, value);
				return true;
			} else {
				matrix->writeRow(rowIndex, &value, 1);
				return true;
			}
		}

		bool contains(const K &value) {
			size_t rowIndex = hashEngine->hashCode(value);
			if (rowIndex < matrix->getRowSize()) {
				// пройти по точкам с одинаковым хэшкодом (значения коллизий)
				Row &row = (*matrix)[rowIndex];
				for (size_t i = 0; i < row.pointSize; ++i) {
					if (row[i] == value) {
						return true;
					}
				}
				return false;
			} else {
				return false;
			}
		}

	private:
		utils::RDMatrix<bool, K> *matrix;
		hash::HashEngine<K> *hashEngine;
		size_t keySizeof; // saved value sizeof Entry
	};
}

#endif
