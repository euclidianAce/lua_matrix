/* Utility Functions and Macros
 * - Basic type checking
 * - Indexing into arrays with certain structure
 * - Performing operations on arrays given certain structure
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "typedefs.h"

#define SIZE_ERR "Matrices not same size"
#define ALLOC_FAIL(L) luaL_error(L, "Unable to allocate memory.")

#define max(x, y) ((x < y) ? y : x)

#define get_row_from_index(index, col) (index / col + 1)
#define get_col_from_index(index, col) (index % col + 1)
#define get_index(cols, row, col) (cols * (row - 1) + col - 1)

// assuming v_ptr is definitely a vector
#define vec_len(v_ptr) max(v_ptr->rows, v_ptr->cols)
#define row_vec(v_ptr) (v_ptr->rows > v_ptr->cols)
#define col_vec(v_ptr) (v_ptr->cols > v_ptr->rows)

int same_size(Matrix *m1, Matrix *m2);
Matrix *is_matrix(lua_State *L, int index);
double dot(double *arr1, double *arr2, size_t size);

// Multiplies arr1 and arr2 into result as if they were matrices of the given sizes.
// Does no size checking or memory allocation.
void multiply(
	double *arr1, int rows1, int cols1, 
	double *arr2, int cols2, 
	double *result
);
void arr_add(int start, int end, double *arr1, double *arr2, double *result);
void arr_sub(int start, int end, double *arr1, double *arr2, double *result);
void arr_unm(int start, int end, double *arr1, double *result);
void arr_div(int start, int end, double *arr, double divisor, double *result);
void arr_idiv(int start, int end, double *arr, double divisor, double *result);
void copy(int start, int end, double *arr1, double *arr2);

#endif
