#include "utils.h"

int same_size(Matrix *m1, Matrix *m2) {
	return m1->rows == m2->rows && m1->cols == m2->cols; 
}
Matrix *is_matrix(lua_State *L, int index) {
	void *ud = luaL_checkudata(L, index, METATABLE);
	luaL_argcheck(L, ud != NULL, 1, "`matrix' expected");
	return (Matrix *)ud;
}

double dot(double *arr1, double *arr2, size_t size) {
	double result = 0;
	for(int i = 0; i < size; i++)
		result += arr1[i] * arr2[i];
	return result;
}

// takes two arrays and multiplies them as though they were matrices
// doesn't check size or allocate memory, only puts values where they should go in the result
void multiply(
	double *arr1, int rows1, int cols1, 
	double *arr2, int cols2, 
	double *result
) {
	// for each entry of the result matrix: dot the row of arr1 with the column of arr2
	for(int i = 0; i < rows1 * cols2; i++) {
		result[i] = 0;
		// row of the first matrix (i.e. the offset to be added to)
		int row_index = (i / cols2) * cols1;
		// column of the second matrix (i.e. the offset to be added to)
		int col_index = (i % cols2);
		for(; col_index < cols1*cols2; row_index+=1, col_index+=cols2)
			result[i] += arr1[ row_index ] * arr2[ col_index ];
	}
}

void arr_add(int start, int end, double *arr1, double *arr2, double *result) {
	for(int i = start; i < end; i++)
		result[i] = arr1[i] + arr2[i];
}
void arr_sub(int start, int end, double *arr1, double *arr2, double *result) {
	for(int i = start; i < end; i++)
		result[i] = arr1[i] - arr2[i];
}
void arr_unm(int start, int end, double *arr1, double *result) {
	for(int i = start; i < end; i++)
		result[i] = -arr1[i];
}
void arr_div(int start, int end, double *arr, double divisor, double *result) {
	for(int i = start; i < end; i++)
		result[i] = arr[i] / divisor;
}
void arr_idiv(int start, int end, double *arr, double divisor, double *result) {
	for(int i = start; i < end; i++)
		result[i] = (int)arr[i] / divisor;
}
void copy(int start, int end, double *arr1, double *arr2) {
	for(int i = start; i < end; i++)
		arr2[i] = arr1[i];
}

