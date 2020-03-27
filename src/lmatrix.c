#include "lmatrix.h"

static const struct luaL_Reg matrix_funcs[] = {
	{"new",	lua_make_matrix},
	{"identity", lua_make_identity_matrix},
	{"random", make_random_matrix},
	{"translation", lua_translation_matrix},
	{"mainRotation", lua_main_rotation_matrix},
	{"rotation", lua_rotation_matrix},
	{NULL, NULL}
};

static const struct luaL_Reg vector_funcs[] = {
	{"newRow", lua_make_row_vector},
	{"newCol", lua_make_col_vector},
	{NULL, NULL}
};

static const struct luaL_Reg matrix_metamethods[] = {
	{"__add", matrix_add},
	{"__sub", matrix_sub},
	{"__unm", matrix_unm},
	{"__div", matrix_div},
	{"__idiv", matrix_idiv},
	{"__mod", matrix_mod},
	{"__mul", matrix_mul},
	{"__pow", matrix_pow},
	{"__gc", matrix_gc},
	{"__tostring", matrix_tostring},
	{"__index", matrix_index},
	{NULL, NULL}
};

static const struct luaL_Reg matrix_methods[] = {
	{"size",lua_get_matrix_size},
	{"rows",generate_rows},
	{"cols",generate_cols},
	{"entries", generate_entries},
	{"map", matrix_map},
	{"schur", matrix_schur},

	// Vector specific methods
	{"magnitude", vector_magnitude},
	{"squareMag", vector_magnitude_squared},
	{"normalize", vector_normalize},
	{"dot", vector_dot},
	{"cross", vector_cross},
	{NULL, NULL}
};

int luaopen_lmatrix(lua_State *L) {
	luaL_newmetatable(L, METATABLE);
	luaL_setfuncs(L, matrix_metamethods, 0);

	lua_pushliteral(L, "methods");
	lua_newtable(L);
	luaL_setfuncs(L, matrix_methods, 0);
	lua_settable(L, -3);

	lua_newtable(L);
	luaL_setfuncs(L, matrix_funcs, 0);
	lua_pushliteral(L, "vector");
	lua_newtable(L);
	luaL_setfuncs(L, vector_funcs, 0);
	lua_settable(L, -3);
	return 1;
}
