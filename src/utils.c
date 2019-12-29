#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "typedefs.h"

// I will never remember which one is which
int get_row_from_index(int index, int col) {return index / col + 1;}
int get_col_from_index(int index, int col) {return index % col + 1;}

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
	return &m->val[ m->cols * (row-1) + col - 1 ];
}

// takes two arrays and multiplies them as though they were matrices
// doesn't check size or allocate memory, only puts values where they should go in the result
void multiply(double *arr1, int rows1, int cols1, 
	      double *arr2, int cols2, 
	      double *result) 
{
	for(int i = 0; i < rows1 * cols2; i++) {
		result[i] = 0;
		for(int j = 0; j < cols2; j++) {
			int index1 = (i / cols2) * cols1 + j;
			int index2 = (i % cols2) + j * cols2;
			result[i] += arr1[ index1 ] * arr2[ index2 ];
		}
	}
}
