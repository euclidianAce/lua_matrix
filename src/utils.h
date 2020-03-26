/* Utility Functions and Macros
 * - Basic type checking
 * - Indexing into arrays with certain structure
 * - Performing operations on arrays given certain structure
 */
#ifndef UTILS_H
#define UTILS_H

#include "typedefs.h"

#include <stdio.h>
#include <lauxlib.h>

// Quickly give an error to the lua state about memory allocation
#define ALLOC_FAIL(L) luaL_error(L, "Unable to allocate memory.")

#define SIZE_ERR "Matrices not same size"
// Indexing functions
#define get_row_from_index(index, col) (index / col + 1)
#define get_col_from_index(index, col) (index % col + 1)
#define get_index(cols, row, col) (cols * (row - 1) + col - 1)

int same_size(Matrix *m1, Matrix *m2);
// Checks if a given index on the Lua stack is a matrix (via metatables) and returns it, errors otherwise
Matrix *is_matrix(lua_State *L, int index);

// gets the address of a matrix element from the Lua stack
double *get_element_addr(lua_State *L);

double dot(double *arr1, double *arr2, size_t size);

// Multiplies arr1 and arr2 into result as if they were matrices of the given sizes.
// Does no size checking or memory allocation.
void multiply(double *arr1, int rows1, int cols1,
	      double *arr2, int cols2,
	      double *result);

// Copies a range of values from arr1 into arr2
void copy(int start, int end, double *arr1, double *arr2);

#endif
