#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
#include "utils/SMatrixTest.h"
#include "utils/IntBitSetTest.h"
#include "utils/CHashSetTest.h"
#include "utils/matrix_utils_test.h"
#include "matrix/DeterminantTest.h"
//#include "ml/DigitalFilterTest.h"

int main(int argc, char **argv) {

//    char* curloc = setlocale(LC_ALL,"");
//    wprintf(L"%s\n", curloc);


    linkedList_test();
    cArrayList_test();
    cMatrix_test();
    test::utils_smatrix::sMatrix_test();
    test::matrix_determinant::determinant_test();
    test::utils_intBitSet::intBitSet_test();
    test::utils_cHashSet::cHashSet_test();
    matrix_utils_test();
//	digitalFilter_test();

    return 0;

}
