#include <CMatrixTest.h>

void test_set_value() {
	size_t rowSize = 4;
	size_t colSize = 3;
	utils::CMatrix<int> matrix(rowSize, colSize);

	for (size_t r=0; r<rowSize; ++r) {
		int val = r * colSize;
		int values[] = {val, val+1, val+2};
		matrix.writeRow(r, values);
	}

	matrix.printMatrix();


}

void cMatrix_test() {
	suite("CMatrix");
	test(set_value);
}
