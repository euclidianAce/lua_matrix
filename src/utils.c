#include "lua-5.3.5/src/lua.h"
#include "lua-5.3.5/src/lualib.h"
#include "lua-5.3.5/src/lauxlib.h"
#include "typedefs.h"

// I will never remember which one is which
int get_row_from_index(int index, int col) {return index / col + 1;}
int get_col_from_index(int index, int col) {return index % col + 1;}

Matrix *is_matrix(lua_State *L, int index) {
	void *ud = luaL_checkudata(L, index, METATABLE);
	luaL_argcheck(L, ud != NULL, 1, "`matrix' expected");
	return (Matrix *)ud;
}

// util function for set and get
double *get_element_addr(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	int row = luaL_checkinteger(L, 2);
	int col = luaL_checkinteger(L, 3);

	// validate arguments
	luaL_argcheck(L, m != NULL, 1 ,"`matrix' expected");
	luaL_argcheck(L, 1 <= row && row <= m->rows, 2, "row out of range");
	luaL_argcheck(L, 1 <= col && col <= m->cols, 3, "column out of range");
	
	// return the address
	return &m->val[ m->cols * (row-1) + col - 1 ];
}
