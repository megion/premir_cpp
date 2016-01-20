#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
#include "utils/SMatrixTest.h"
#include "utils/ArrayUtilsTest.h"
#include "utils/IntBitSetTest.h"
#include "utils/CHashSetTest.h"
#include "utils/matrix_utils_test.h"
#include "matrix/DeterminantTest.h"
#include "file/FileReaderTest.h"
#include "sspy/SspyTest.h"
#include "kohonen/NetworkInitializerTest.h"
#include "ml/DigitalFilterTest.h"

int main(int argc, char **argv) {

//    char* curloc = setlocale(LC_ALL,"");
//    wprintf(L"%s\n", curloc);


    linkedList_test();
    cArrayList_test();
    cMatrix_test();
    test::utils_arrayutils::arrayutils_test();
    test::utils_smatrix::sMatrix_test();
    test::utils_intBitSet::intBitSet_test();
    test::utils_cHashSet::cHashSet_test();
    test::matrix_determinant::determinant_test();
    test::file_reader::file_reader_test();
    test::sspy::sspy_data_read_test();
    test::kohonen_initializer::network_initializer_test();
    matrix_utils_test();
	digitalFilter_test();

    return 0;

}
