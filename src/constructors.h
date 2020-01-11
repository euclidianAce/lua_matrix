#ifndef _CONSTRUCTORS_H
#define _CONSTRUCTORS_H
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <math.h>

#include "utils.h"
#include "typedefs.h"
#include "constructors.h"

Matrix *make_matrix(lua_State *L, int rows, int cols);
void make_identity_array(int size, double *result);
Matrix *make_identity_matrix(lua_State *L, int size);

#endif
