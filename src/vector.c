#include "vector.h"

int lua_make_row_vector(lua_State *L) {
	if(lua_isinteger(L, 1)) {
		int cols = luaL_checkinteger(L, 1);
		make_row_vector(L, cols);
		return 1;
	}
	
	if(lua_istable(L, 1)) {
		int len = luaL_len(L, 1);
		Matrix *m = make_row_vector(L, len);
		for(int i = 0; i < len; i++) {
			if(lua_geti(L, 1, i+1) != LUA_TNUMBER) {
				return luaL_argerror(L, 1, "Bad table value, number expected");
			}
			m->val[i] = lua_tonumber(L, -1);
		}
		return 1;
	}
	return luaL_argerror(L, 1, "Expected table or integer");
}

int lua_make_col_vector(lua_State *L) {
	if(lua_isinteger(L, 1)) {
		int rows = luaL_checkinteger(L, 1);
		make_col_vector(L, rows);
		return 1;
	}
	
	if(lua_istable(L, 1)) {
		int len = luaL_len(L, 1);
		Matrix *m = make_col_vector(L, len);
		for(int i = 0; i < len; i++) {
			if(lua_geti(L, 1, i+1) != LUA_TNUMBER) {
				return luaL_argerror(L, 1, "Bad table value, number expected");
			}
			m->val[i] = lua_tonumber(L, -1);
		}
		return 1;
	}
	return luaL_argerror(L, 1, "Expected table or integer");
}


