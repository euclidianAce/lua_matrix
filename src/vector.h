#ifndef VECTOR_H
#define VECTOR_H
#include "constructors.h"

#define make_row_vector(L, cols) make_matrix(L, 1, cols)
#define make_col_vector(L, rows) make_matrix(L, rows, 1)

int lua_make_row_vector(lua_State *L);
int lua_make_col_vector(lua_State *L);

#endif
