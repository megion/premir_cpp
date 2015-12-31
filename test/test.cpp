#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
#include "utils/SMatrixTest.h"
#include "utils/IntBitSetTest.h"
#include "utils/matrix_utils_test.h"
#include "matrix/DeterminantTest.h"
//#include "ml/DigitalFilterTest.h"

int main(int argc, char **argv) {

    linkedList_test();
    cArrayList_test();
    cMatrix_test();
    test::utils_smatrix::sMatrix_test();
    test::matrix_determinant::determinant_test();
    test::utils_intBitSet::intBitSet_test();
    matrix_utils_test();
//	digitalFilter_test();

    return 0;

}
