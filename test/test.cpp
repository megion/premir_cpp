#include "utils/LinkedListTest.h"
#include "utils/CArrayListTest.h"
#include "utils/CMatrixTest.h"
#include "utils/SMatrixTest.h"
#include "utils/RMatrixTest.h"
#include "utils/RDMatrixTest.h"
#include "utils/R3DMatrixTest.h"
#include "utils/ArrayUtilsTest.h"
#include "utils/IntBitSetTest.h"
#include "utils/CHashSetTest.h"
#include "utils/matrix_utils_test.h"
#include "matrix/DeterminantTest.h"
#include "matrix/GramSchmidtTest.h"
#include "file/FileReaderTest.h"
#include "file/CsvFileSummaryTest.h"
#include "sspy/SspyTest.h"
#include "kohonen/KohonenTest.h"
#include "kohonen/KohonenDemosTest.h"
#include "coursera/CourseraTest.h"
#include "ml/DigitalFilterTest.h"

int main(int argc, char **argv) {

//    char* curloc = setlocale(LC_ALL,"");
//    wprintf(L"%s\n", curloc);


    linkedList_test();
    cArrayList_test();
    cMatrix_test();
    test::utils_arrayutils::arrayutils_test();
    test::utils_smatrix::sMatrix_test();
    test::utils_rmatrix::rMatrix_test();
    test::utils_rdmatrix::rDMatrix_test();
    test::utils_r3dmatrix::r3DMatrix_test();
    test::utils_intBitSet::intBitSet_test();
    test::utils_cHashSet::cHashSet_test();
    test::matrix_determinant::determinant_test();
    test::matrix_gramSchmidt::gramSchmidt_test();
    test::file_reader::file_reader_test();
    test::sspy::sspy_data_read_test();
    test::kohonen_test::kohonen_test();
    test::kohonen_demos_test::kohonen_demos_test();
    test::file_csvfilesummary::file_csvFileSummary_test();
    test::coursera::coursera_test();
    matrix_utils_test();
	digitalFilter_test();

    return 0;

}
