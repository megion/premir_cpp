#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
#include "utils/SMatrixTest.h"
#include "utils/RMatrixTest.h"
#include "utils/RDMatrixTest.h"
#include "utils/R3DMatrixTest.h"
#include "utils/HashEngineTest.h"
#include "utils/HashSetTest.h"
#include "utils/HashMapTest.h"
#include "utils/HashMapArrayTest.h"
#include "utils/ArrayUtilsTest.h"
#include "utils/IntBitSetTest.h"
#include "utils/matrix_utils_test.h"
#include "matrix/DeterminantTest.h"
#include "matrix/GramSchmidtTest.h"
#include "file/FileReaderTest.h"
#include "file/CsvFileSummaryTest.h"
#include "kohonen/KohonenTest.h"
#include "kohonen/KohonenDemosTest.h"
#include "coursera/CourseraTest.h"
#include "comb/CombinatoricsTest.h"
#include "comb/TypingTextGenerator.h"
#include "ml/DigitalFilterTest.h"
#include "ml/LevensteinDistanceTest.h"

#include "cache/LinkedHashMapTest.h"
#include "cache/StringListTest.h"


#include "apuebook/apue-tests.h"

int main(int argc, char **argv) {

//    char* curloc = setlocale(LC_ALL,"");
//    wprintf(L"%s\n", curloc);


    linkedList_test();
	test::utils_carray_list::cArrayList_test();
    cMatrix_test();
    test::utils_arrayutils::arrayutils_test();
    test::utils_smatrix::sMatrix_test();
    test::utils_rmatrix::rMatrix_test();
    test::utils_rdmatrix::rDMatrix_test();
    test::utils_r3dmatrix::r3DMatrix_test();
	test::utils_hash_engine::hashEngine_test();
    test::utils_hash_map::hashMap_test();
    test::utils_hash_map_array::hashMapArray_test();
    test::utils_intBitSet::intBitSet_test();
    test::utils_hash_set::hashSet_test();
    test::matrix_determinant::determinant_test();
    test::matrix_gramSchmidt::gramSchmidt_test();
    test::file_reader::file_reader_test();
    test::kohonen_test::kohonen_test();

    test::file_csvfilesummary::file_csvFileSummary_test();

    matrix_utils_test();
    test::ml_digital_filter::digital_filter_test();
    test::ml_digital_filter::decision_tree_test();
    test::ml_levenstein_distance::livenstein_distance_test();

    test::kohonen_demos_test::kohonen_demos_test();

    test::coursera::coursera_test();

    test::combinatorics::combinatorics_test();
    test::combinatorics::generate_text_test();

	test::cache_linked_hash_map::linkedHashMap_test();
	test::cache_string_list::stringList_test();

    //test::apuebook::fork2_test();

    return 0;

}
