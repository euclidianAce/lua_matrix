#ifndef VECTOR_H
#define VECTOR_H
#include "constructors.h"

Matrix *make_row_vector(lua_State *L, int cols);
Matrix *make_col_vector(lua_State *L, int rows);
int lua_make_row_vector(lua_State *L);
int lua_make_col_vector(lua_State *L);
#endif
