// a matrix library for Lua 5.3.5

#include "lmatrix.h"

static const struct luaL_Reg matrixlib_funcs[] = {
	{"new", 	lua_make_matrix		},
	{"identity", 	lua_make_identity_matrix},
	{"random", 	make_random_matrix	},
	{"translation",	lua_translation_matrix	},
	{"mainRotation",lua_main_rotation_matrix},
	{"rotation",	lua_rotation_matrix	},
	{NULL, 		NULL			}
};

static const struct luaL_Reg vector_funcs[] = {
	{"newRow",	lua_make_row_vector	},
	{"newCol",	lua_make_col_vector	},
	{NULL, 		NULL			}
};

static const struct luaL_Reg matrixlib_meta_index[] = {
	{"set",		lua_set_matrix_element	},
	{"get",		lua_get_matrix_element	},
	{"size",	lua_get_matrix_size	},
	{"rows", 	generate_rows		},
	{"cols", 	generate_cols		},
	{"entries", 	generate_entries	},
	{"map", 	matrix_map		},
	{"schur", 	matrix_schur		},

	// Vector methods
	{"magnitude", 	vector_magnitude	},
	{"squareMag", 	vector_magnitude_squared},
	{"normalize", 	vector_normalize	},
	{"dot", 	vector_dot		},
	{"cross", 	vector_cross		},

	{NULL, 		NULL			}
};

static const struct luaL_Reg matrixlib_metamethods[] = {
	{"__add", 	matrix_add		},
	{"__sub", 	matrix_sub		},
	{"__unm", 	matrix_unm		},
	{"__div", 	matrix_div		},
	{"__idiv", 	matrix_idiv		},
	{"__mod", 	matrix_mod		},
	{"__mul", 	matrix_mul		},
	{"__pow",	matrix_pow		},
	{"__gc", 	matrix_gc		},
	{NULL, NULL}
};

// this basically get called when require("lua_matrix") is called
int luaopen_lmatrix(lua_State *L) {
	luaL_newmetatable(L, METATABLE);
	// make the .__index entry in the metatable
	lua_pushstring(L, "__index");		// push the key __index onto the stack
	lua_newtable(L); 			// push a new empty table onto the stack
	luaL_setfuncs(L, matrixlib_meta_index, 0); // fill it with the index methods
	lua_settable(L, -3); 			// METATABLE["__index"] = matrix_m_index

	// make the rest of the metatable (__add and stuff)
	luaL_setfuncs(L, matrixlib_metamethods, 0);

	lua_newtable(L); 			// push an empty table onto the stack
	luaL_setfuncs(L, matrixlib_funcs, 0); 	// fill it with our functions
	lua_pushstring(L, "vector");
	lua_newtable(L);
	luaL_setfuncs(L, vector_funcs, 0);
	lua_settable(L, -3);
	return 1;				// return it to lua
}
