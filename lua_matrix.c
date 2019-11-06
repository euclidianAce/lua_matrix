// a matrix library for lua

#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>

#include<stdlib.h>
#include<math.h>

#define METATABLE "Matrix"


typedef struct Matrix {
	int rows, cols;
	double val[1];
} Matrix;


static Matrix *is_matrix(lua_State *L) {
	void *ud = luaL_checkudata(L, 1, METATABLE);
	luaL_argcheck(L, ud != NULL, 1, "`matrix' expected");
	return (Matrix *)ud;
}


//{{{ Constructors
static int make_matrix(lua_State *L) {
	// takes 2 integer arguments, rows and columns
	int rows = luaL_checkinteger(L, 1);
	int cols = luaL_checkinteger(L, 2);
	
	// a matrix is basically a big array of doubles with rows*cols elements
	size_t nbytes = sizeof(Matrix) + (rows * cols) * sizeof(double);
	Matrix *m = (Matrix *)lua_newuserdata(L, nbytes);

	// set the metatable to make it a matrix object
	luaL_setmetatable(L, METATABLE);

	m->rows = rows;
	m->cols = cols;

	return 1;
}
// add identity and random constructors

//}}}

//{{{ Getters and Setters


// util function for set and get
static double *get_element_addr(lua_State *L) {
	Matrix *m = is_matrix(L);
	int row = luaL_checkinteger(L, 2);
	int col = luaL_checkinteger(L, 3);

	// validate arguments
	luaL_argcheck(L, m != NULL, 1 ,"`matrix' expected");
	luaL_argcheck(L, 1 <= row && row <= m->rows, 2, "row out of range");
	luaL_argcheck(L, 1 <= col && col <= m->cols, 3, "column out of range");
	
	// return the address
	return &m->val[ m->cols * (row-1) + col - 1 ];
}

// set elements
static int set_matrix_element(lua_State *L) {
	double val = luaL_checknumber(L, 4);
	
	*get_element_addr(L) = val;
	return 0;
}

// get elements
static int get_matrix_element(lua_State *L) {
	lua_pushnumber(L, *get_element_addr(L));
	return 1;
}


// accessing the size in lua
static int get_size(lua_State *L) {
	Matrix *m = is_matrix(L);
	lua_pushnumber(L, m->rows);
	lua_pushnumber(L, m->cols);
	return 2;
}

// }}}


// initialize the library
static const struct luaL_reg matrixlib_f [] = {
	{"new", make_matrix},
	{NULL, NULL}
};

// metamethods for matrix object
static const struct luaL_reg matrixlib_m [] = {
	{"set", set_matrix_element},
	{"get", get_matrix_element},
	{NULL, NULL}
};

// this basically get called when require("lua_matrix") is called
int luaopen_lua_matrix(lua_State *L) {
	luaL_newmetatable(L, METATABLE);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_setfuncs(L, matrixlib_m, 0);

	lua_newtable(L);
	luaL_setfuncs(L, matrixlib_f, 0);
	return 1;
}


