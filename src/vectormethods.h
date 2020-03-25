#ifndef VEC_METHOD_H
#define VEC_METHOD_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <math.h>

#include "constructors.h"
#include "utils.h"

int vector_dot(lua_State *L);
int vector_cross(lua_State *L);
int vector_magnitude_squared(lua_State *L);
int vector_magnitude(lua_State *L);
int vector_normalize(lua_State *L);

#endif
