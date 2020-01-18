/*
 * --------------------------------------------------------------------------------------------------
 * --   Implementation of the Aguilera-Perez Algorithm                                             --
 * --   Aguilera, Antonio, and Ricardo Perez-Aguilera. "General n-dimensional rotations." (2004)   --
 * --   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.4.8662                             --
 * --------------------------------------------------------------------------------------------------
 */
#ifndef _ROTATION_H
#define _ROTATION_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "constructors.h"
#include "typedefs.h"
#include "utils.h"

int lua_translation_matrix(lua_State *L);
int lua_main_rotation_matrix(lua_State *L);
int lua_rotation_matrix(lua_State *L);

#endif
