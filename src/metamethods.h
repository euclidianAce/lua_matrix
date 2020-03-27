#ifndef METAMETHODS_H
#define METAMETHDOS_H
#include <lua.h>
#include <lauxlib.h>

#include <stdlib.h>
#include <math.h>

#include "constructors.h"
#include "iterators.h"
#include "methods.h"
#include "typedefs.h"
#include "utils.h"

int matrix_add(lua_State *L);
int matrix_sub(lua_State *L);
int matrix_unm(lua_State *L);
int matrix_div(lua_State *L);
int matrix_idiv(lua_State *L);
int matrix_mod(lua_State *L);
int matrix_mul(lua_State *L);
int matrix_pow(lua_State *L);
int matrix_gc(lua_State *L);
int matrix_tostring(lua_State *L);
int matrix_index(lua_State *L);

#endif
