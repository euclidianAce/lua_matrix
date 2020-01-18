#include "iterators.h"

int rows(lua_State *L) {
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
		lua_replace(L, lua_upvalueindex(2)); //update upvalue
		return 2;
	} else {
		lua_pushnil(L);
		return 1;
	}
	return 0;
}

int generate_rows(lua_State *L) {
	is_matrix(L, 1);			// validate that the first argument is a matrix
	lua_pushinteger(L, 0);			// push the index 0 to the stack
	lua_pushcclosure(L, rows, 2);		// push the rows function onto the stack with 2 upvalues
	return 1;
}

int cols(lua_State *L) {
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
		lua_replace(L, lua_upvalueindex(2)); //update upvalue
		return 2;
	} else {
		lua_pushnil(L);
		return 1;
	}
	return 0;
}
int generate_cols(lua_State *L) {
	is_matrix(L, 1);			// validate that the top of the stack is a matrix
	lua_pushinteger(L, 0);			// push the index 0 to the stack
	lua_pushcclosure(L, cols, 2);		// push the cols function onto the stack with 2 upvalues
	return 1;
}

int entries(lua_State *L) {
	Matrix *m = lua_touserdata(L, lua_upvalueindex(1));
	int currentIndex = lua_tointeger(L, lua_upvalueindex(2));

	if( currentIndex < m->rows * m->cols ) {
		lua_pushinteger(L, get_row_from_index(currentIndex, m->cols));	// push the current row
		lua_pushinteger(L, get_col_from_index(currentIndex, m->cols));	// push the current column
		lua_pushnumber(L, m->val[currentIndex]);			// push the entry

		lua_pushinteger(L, ++currentIndex);				// update the upvalue
		lua_replace(L, lua_upvalueindex(2));
		return 3;
	} else {
		lua_pushnil(L);
		return 1;
	}
	return 0;
}

int generate_entries(lua_State *L) {
	is_matrix(L, 1);			// validate that the top of the stack is a matrix
	lua_pushinteger(L, 0);			// push the index 0 to the stack
	lua_pushcclosure(L, entries, 2);	// push the entries function onto the stack with 2 upvalues
	return 1;
}
