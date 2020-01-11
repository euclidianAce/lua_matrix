#ifndef UTILS_H
#define UTILS_H

#include "typedefs.h"
#include <lauxlib.h>

#define ALLOC_FAIL(L) luaL_error(L, "Unable to allocate memory.")

int get_row_from_index(int index, int col);
int get_col_from_index(int index, int col);
int get_index(int rows, int row, int col);
Matrix *is_matrix(lua_State *L, int index);
double *get_element_addr(lua_State *L);
void multiply(double *arr1, int rows1, int cols1,
	      double *arr2, int cols2,
	      double *result);
void copy(int start, int end, double *arr1, double *arr2);

#endif
