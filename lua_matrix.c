// a matrix library for lua

#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>

#include<stdlib.h>
#include<stdbool.h>

#include<math.h>

#define METATABLE "Matrix"


typedef struct Matrix {
	int rows, cols;
	double val[1];
} Matrix;


static Matrix *is_matrix(lua_State *L, int index) {
	void *ud = luaL_checkudata(L, index, METATABLE);
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
	Matrix *m = is_matrix(L, 1);
	lua_pushnumber(L, m->rows);
	lua_pushnumber(L, m->cols);
	return 2;
}

// }}}

//{{{ Metamethods

static bool same_size(Matrix *m1, Matrix *m2) {
	return m1->rows == m2->rows && m1->cols == m2->cols; 
}

static int matrix_add(lua_State *L) {
	// check if the two arguments are matrices and assign them
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	
	// check if m1 and m2 are the same size
	luaL_argcheck(L, same_size(m1, m2), 2, "Matrices not the same size");
	
	// make a new matrix to return
	size_t nbytes = sizeof(Matrix) + (m1->rows * m1->cols) * sizeof(double);
	Matrix *sum = (Matrix *)lua_newuserdata(L, nbytes);
	
	// set the size of it
	sum->rows = m1->rows;
	sum->cols = m1->cols;

	// actually do the adding
	for(int i = 0; i < m1->rows * m1->cols - 1; i++)
		sum->val[i] = m1->val[i] + m2->val[i];

	// set the metatable for the sum
	luaL_setmetatable(L, METATABLE);

	return 1; //return the matrix
}

//}}}

// initialize the library
static const struct luaL_reg matrixlib_f [] = {
	{"new", make_matrix},
	{NULL, NULL}
};

// default methods for matrix object
static const struct luaL_reg matrixlib_m_index [] = {
	{"set", set_matrix_element},
	{"get", get_matrix_element},
	{NULL, NULL}
};

// metamethods for matrix object
static const struct luaL_reg matrixlib_metam [] = {
	{"__add", matrix_add},
	{NULL, NULL}
};

// this basically get called when require("lua_matrix") is called
int luaopen_lua_matrix(lua_State *L) {
	luaL_newmetatable(L, METATABLE);
	// make the .__index entry in the metatable
	lua_pushstring(L, "__index");		// push the key __index onto the stack
	lua_newtable(L); 			// push a new empty table onto the stack
	luaL_setfuncs(L, matrixlib_m_index, 0); // fill it with the index methods
	lua_settable(L, -3); 			// METATABLE["__index"] = matrix_m_index
	// make the rest of the metatable (__add and stuff)
	luaL_setfuncs(L, matrixlib_metam, 0);

	lua_newtable(L); 			// push an empty table onto the stack
	luaL_setfuncs(L, matrixlib_f, 0); 	// fill it with our functions
	return 1;				// return it to lua
}


