#ifndef ITERATORS_H
#define ITERATORS_H

#include <lua.h>
#include <lauxlib.h>

#include "typedefs.h"
#include "utils.h"

int generate_rows(lua_State *L) ;
int generate_cols(lua_State *L);
int generate_entries(lua_State *L);

#endif
