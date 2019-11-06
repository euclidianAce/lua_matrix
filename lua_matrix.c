// a matrix library for lua


#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>

#include<stdlib.h>
#include<math.h>

typedef struct Matrix {
	int rows, cols;
	double val[1];
} Matrix;

// function to define a matrix
static int make_matrix(lua_State *L) {
	// takes 2 integer arguments, rows and columns
	int rows = luaL_checkinteger(L, 1);
	int cols = luaL_checkinteger(L, 2);
	
	// a matrix is basically a big array of doubles with rows*cols elements
	size_t nbytes = sizeof(Matrix) + (rows * cols - 1) * sizeof(double);
	Matrix *m = (Matrix *)lua_newuserdata(L, nbytes);

	m->rows = rows;
	m->cols = cols;

	return 1;
}

// functions to access matrix elements

// set elements
static int set_matrix_element(lua_State *L) {
	Matrix *m = (Matrix *)lua_touserdata(L, 1);
	int row = luaL_checkinteger(L, 2);
	int col = luaL_checkinteger(L, 3);
	double val = luaL_checknumber(L, 4);

	// validate arguments
	luaL_argcheck(L, m != NULL, 1 ,"`matrix' expected");
	luaL_argcheck(L, 1 <= row && row <= m->rows, 2, "row out of range");
	luaL_argcheck(L, 1 <= col && col <= m->cols, 3, "column out of range");

	// make the assignment
	m->val[row*col-1] = val;
	return 0;
}

// get elements
static int get_matrix_element(lua_State *L) {
	Matrix *m = (Matrix *)lua_touserdata(L, 1);
	int row = luaL_checkinteger(L, 2);
	int col = luaL_checkinteger(L, 3);
	
	// validate arguments
	luaL_argcheck(L, m != NULL, 1 ,"`matrix' expected");
	luaL_argcheck(L, 1 <= row && row <= m->rows, 2, "row out of range");
	luaL_argcheck(L, 1 <= col && col <= m->cols, 3, "column out of range");


	// push value onto the stack
	lua_pushnumber(L, m->val[row*col-1]);
	return 1;
}





// initialize the library
static const struct luaL_reg matrixlib [] = {
	{"new", make_matrix},
	{"set", set_matrix_element},
	{"get", get_matrix_element},
	{NULL, NULL}
};

int luaopen_lua_matrix(lua_State *L) {
	lua_newtable(L);
	luaL_setfuncs(L, matrixlib, 0);
	return 1;
}


