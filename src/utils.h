#ifndef _UTILS_H
#define _UTILS_H
#include "typedefs.h"

int get_row_from_index(int index, int col);
int get_col_from_index(int index, int col);
Matrix *is_matrix(lua_State *L, int index);
double *get_element_addr(lua_State *L);
#endif
