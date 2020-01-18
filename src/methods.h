#ifndef METHODS_H
#define METHODS_H

#include <lua.h>
#include <lauxlib.h>

#include "constructors.h"
#include "typedefs.h"
#include "utils.h"


int lua_set_matrix_element(lua_State *L);
int lua_get_matrix_element(lua_State *L);
int lua_get_matrix_size(lua_State *L);
int matrix_schur(lua_State *L);
int matrix_map(lua_State * L);

#endif