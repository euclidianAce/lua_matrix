#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "utils.h"
#include "typedefs.h"


inline int get_row_from_index(int index, int col) 	{return index / col + 1;}
inline int get_col_from_index(int index, int col) 	{return index % col + 1;}
inline int get_index(int cols, int row, int col)	{return cols * (row - 1) + col - 1;}

Matrix *is_matrix(lua_State *L, int index) {
	void *ud = luaL_checkudata(L, index, METATABLE);
	luaL_argcheck(L, ud != NULL, 1, "`matrix' expected");
	return (Matrix *)ud;
}

// util function for set and get
double *get_element_addr(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	int row = luaL_checkinteger(L, 2);
	int col = luaL_checkinteger(L, 3);

	// validate arguments
	luaL_argcheck(L, m != NULL, 1 ,"`matrix' expected");
	luaL_argcheck(L, 1 <= row && row <= m->rows, 2, "row out of range");
	luaL_argcheck(L, 1 <= col && col <= m->cols, 3, "column out of range");
	
	// return the address
	return &m->val[ get_index(m->cols, row, col) ];
}

double dot(double *arr1, double *arr2, size_t size) {
	double result = 0;
	for(int i = 0; i < size; i++)
		result += arr1[i] * arr2[i];
	return result;
}

// takes two arrays and multiplies them as though they were matrices
// doesn't check size or allocate memory, only puts values where they should go in the result
void multiply(double *arr1, int rows1, int cols1, 
	      double *arr2, int cols2, 
	      double *result) 
{
	// for each entry of the result matrix: dot the row of arr1 with the column of arr2
	for(int i = 0; i < rows1 * cols2; i++) {
		result[i] = 0;
		// row of the first matrix (i.e. the offset to be added to)
		int row_index = (i / cols2) * cols1;
		// column of the second matrix (i.e. the offset to be added to)
		int col_index = (i % cols2);
		for(; col_index < cols1*cols2; row_index+=1, col_index+=cols2) {
			result[i] += arr1[ row_index ] * arr2[ col_index ];
		}
	}
}

void copy(int start, int end, double *arr1, double *arr2) {
	for(int i = start; i < end; i++)
		arr2[i] = arr1[i];
}

