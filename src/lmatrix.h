#ifndef LUA_MATRIX_H
#define LUA_MATRIX_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdlib.h>
#include <math.h>

#include "constructors.h"
#include "iterators.h"
#include "metamethods.h"
#include "methods.h"
#include "rotation.h"
#include "typedefs.h"
#include "utils.h"
#include "vector.h"
#include "vectormethods.h"

int luaopen_lmatrix(lua_State *L);

#endif
