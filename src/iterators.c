#include "iterators.h"

static int rows(lua_State *L) {
	Matrix *m = lua_touserdata(L, lua_upvalueindex(1));
	int currentRow = lua_tointeger(L, lua_upvalueindex(2));
	if( currentRow < m->rows ) {
		lua_pushinteger(L, currentRow + 1);
		lua_newtable(L);
		for(int i = 0; i < m->cols; i++) {
			lua_pushnumber(L, m->val[m->cols*currentRow + i]);
			lua_seti(L, -2, i+1);
		}
		lua_pushinteger(L, ++currentRow);
		lua_replace(L, lua_upvalueindex(2));
		return 2;
	}
	lua_pushnil(L);
	return 1;
}

int generate_rows(lua_State *L) {
	is_matrix(L, 1);
	lua_pushinteger(L, 0);
	lua_pushcclosure(L, rows, 2);
	return 1;
}

static int cols(lua_State *L) {
	Matrix *m = lua_touserdata(L, lua_upvalueindex(1));
	int currentCol = lua_tointeger(L, lua_upvalueindex(2));
	if( currentCol < m->cols ) {
		lua_pushinteger(L, currentCol + 1);
		lua_newtable(L);
		for(int i = 0; i < m->rows; i++) {
			lua_pushnumber(L, m->val[currentCol + i * m->cols]);
			lua_seti(L, -2, i+1);
		}
		lua_pushinteger(L, ++currentCol);
		lua_replace(L, lua_upvalueindex(2));
		return 2;
	}
	lua_pushnil(L);
	return 1;
}

int generate_cols(lua_State *L) {
	is_matrix(L, 1);
	lua_pushinteger(L, 0);
	lua_pushcclosure(L, cols, 2);
	return 1;
}

static int entries(lua_State *L) {
	Matrix *m = lua_touserdata(L, lua_upvalueindex(1));
	int currentIndex = lua_tointeger(L, lua_upvalueindex(2));

	if( currentIndex < m->rows * m->cols ) {
		lua_pushinteger(L, get_row_from_index(currentIndex, m->cols));
		lua_pushinteger(L, get_col_from_index(currentIndex, m->cols));
		lua_pushnumber(L, m->val[currentIndex]);

		lua_pushinteger(L, ++currentIndex);
		lua_replace(L, lua_upvalueindex(2));
		return 3;
	}
	lua_pushnil(L);
	return 1;
}

int generate_entries(lua_State *L) {
	is_matrix(L, 1);
	lua_pushinteger(L, 0);
	lua_pushcclosure(L, entries, 2);
	return 1;
}
