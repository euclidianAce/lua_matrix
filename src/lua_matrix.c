// a matrix library for Lua 5.3.5

#include "lua-5.3.5/src/lua.h"
#include "lua-5.3.5/src/lualib.h"
#include "lua-5.3.5/src/lauxlib.h"

#include<stdlib.h>
#include<math.h>

#define METATABLE "Matrix"
#define SIZE_ERR "Matrices not same size"

typedef struct Matrix {
	int rows, cols;
	double val[1];
} Matrix;


//{{{ Helper Functions

// I will never remember which one is which
static int get_row_from_index(int index, int col) {return index / col + 1;}
static int get_col_from_index(int index, int col) {return index % col + 1;}

static Matrix *is_matrix(lua_State *L, int index) {
	void *ud = luaL_checkudata(L, index, METATABLE);
	luaL_argcheck(L, ud != NULL, 1, "`matrix' expected");
	return (Matrix *)ud;
}

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
//}}}

//{{{ Constructors

// allocates memory, puts matrix userdata on the stack, and returns pointer to matrix
static Matrix *make_matrix(lua_State *L, int rows, int cols) {
	size_t nbytes = sizeof(Matrix) + (rows * cols - 1) * sizeof(double);
	Matrix *m = (Matrix *)lua_newuserdata(L, nbytes);
	m->rows = rows;
	m->cols = cols;
	luaL_setmetatable(L, METATABLE);
	return m;
}

// the constructor called from Lua
static int lua_make_matrix(lua_State *L) {
	// These ifs basically "overload" this function without actually overloading it
	// since it can only take the lua_State as an arg
	if (lua_isinteger(L, -1)) {
		if (lua_isinteger(L, -2)) { /* 2 integers */
			// make a matrix with no initialized values
			int rows = luaL_checkinteger(L, 1);
			int cols = luaL_checkinteger(L, 2);
			make_matrix(L, rows, cols);

		} else if (lua_istable(L, -2)) { /* table and int */
			// make a matrix with values initialized based on the table
			int rows = luaL_checkinteger(L, 2);
			int len = luaL_len(L, -2);
			if( len%rows != 0 )
				return luaL_argerror(L, 2, "Table not evenly divisible");
			Matrix *m = make_matrix(L, rows, len / rows);

			for(int i = 0; i < len; i++) {
				lua_geti(L, -3, i+1);
				if(!lua_isnumber(L, -1))
					return luaL_argerror(L, 1, "Bad table value, number expected");
				m->val[i] = lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
		}
	} else if (lua_istable(L, -1)) { /* table of tables */
		// get the rows and cols to initialize the matrix
		int rows, cols;
		rows = luaL_len(L, -1);

		lua_geti(L, -1, 1);
		if(!lua_istable(L, -1))
			return luaL_argerror(L, 1, "Bad table value, table expected"); 
		cols = luaL_len(L, -1);
		lua_pop(L, 1);
		
		Matrix *m = make_matrix(L, rows, cols);
		for(int i = 0; i < rows; i++) {
			lua_geti(L, -2, i+1);
			if(luaL_len(L, -1) != cols)
				return luaL_argerror(L, 1, "Tables must all be same length"); 

			for(int j = 0; j < cols; j++) {
				lua_geti(L, -1, j+1);
				if(!lua_isnumber(L, -1))
					return luaL_argerror(L, 1, "Bad table value, number expected"); 

				m->val[i*cols + j] = lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}
	} else {
		return luaL_argerror(L, 1, "Table or Integer expected");
	}
	return 1;
}

static int make_identity_matrix(lua_State *L) {
	int rc = luaL_checkinteger(L, 1);
	Matrix *m = make_matrix(L, rc, rc);
	for(int i = 0; i < rc*rc; i+=rc+1)
		m->val[i] = 1;
	return 1;
}

static int make_random_matrix(lua_State *L) {
	int rows = luaL_checkinteger(L, 1);
	int cols = luaL_checkinteger(L, 2);
	Matrix * m = make_matrix(L, rows, cols);
	for(int i = 0; i < rows*cols; i++)
		m->val[i] = (double)rand()/RAND_MAX;
	return 1;
}

//}}}

//{{{ Getters and Setters

static int lua_set_matrix_element(lua_State *L) {
	double val = luaL_checknumber(L, 4);
	*get_element_addr(L) = val;
	return 0;
}

static int lua_get_matrix_element(lua_State *L) {
	lua_pushnumber(L, *get_element_addr(L));
	return 1;
}

static int lua_get_matrix_size(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	lua_pushinteger(L, m->rows);
	lua_pushinteger(L, m->cols);
	return 2;
}
//}}}

//{{{ Iterators

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

static int generate_rows(lua_State *L) {
	Matrix *m = is_matrix(L, 1);		// validate that the top of the stack is a matrix
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
static int generate_cols(lua_State *L) {
	Matrix *m = is_matrix(L, 1);		// validate that the top of the stack is a matrix
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

static int generate_entries(lua_State *L) {
	Matrix *m = is_matrix(L, 1);		// validate that the top of the stack is a matrix
	lua_pushinteger(L, 0);			// push the index 0 to the stack
	lua_pushcclosure(L, entries, 2);	// push the entries function onto the stack with 2 upvalues
	return 1;
}


//}}}

//{{{ Metamethods

static int same_size(Matrix *m1, Matrix *m2) {
	return m1->rows == m2->rows && m1->cols == m2->cols; 
}

static int matrix_add(lua_State *L) {
	// check if the two arguments are matrices and assign them
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	
	// check if m1 and m2 are the same size
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);
	
	// make a new matrix to return
	Matrix *sum = make_matrix(L, m1->rows, m1->cols);

	// actually do the adding
	for(int i = 0; i < m1->rows * m1->cols; i++)
		sum->val[i] = m1->val[i] + m2->val[i];

	return 1; //return the matrix
}

static int matrix_sub(lua_State *L) {
	// check if the two arguments are matrices and assign them
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	
	// check if m1 and m2 are the same size
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);

	// make a new matrix to return
	Matrix *dif = make_matrix(L, m1->rows, m1->cols);

	// actually do the adding
	for(int i = 0; i < m1->rows * m1->cols; i++)
		dif->val[i] = m1->val[i] - m2->val[i];

	return 1; //return the matrix
}

static int matrix_unm(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	Matrix *negm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++)
		negm->val[i] = -m->val[i];

	return 1;

}

static int matrix_div(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double divisor = luaL_checknumber(L, 2);

	Matrix *newm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++) {
		newm->val[i] = m->val[i] / divisor;
	}

	return 1;
}

static int matrix_idiv(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double divisor = luaL_checknumber(L, 2);

	Matrix *newm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++) {
		newm->val[i] = floor(m->val[i] / divisor);
	}

	return 1;
}

static int matrix_mod(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double divisor = luaL_checknumber(L, 2);

	Matrix *newm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++) {
		newm->val[i] = fmod(m->val[i], divisor);
	}

	return 1;
}

static int matrix_mul(lua_State *L) {
	double num;
	Matrix *m1;
	Matrix *m2;

	if(lua_isnumber(L, -1) || lua_isnumber(L, -2)){
		// matrix number product
		if(lua_isnumber(L, -2) /* 1st arg is number */) {
			num = luaL_checknumber(L, 1);
			m1 = is_matrix(L, 2);
		} else if(lua_isnumber(L, -1) /* 2nd arg is number */) {
			num = luaL_checknumber(L, 2);
			m1 = is_matrix(L, 1);
		}
		m2 = make_matrix(L, m1->rows, m1->cols);
		for(int i = 0; i < m1->rows * m1->cols; i++)
			m2->val[i] = num * m1->val[i];
	} else {
		// matrix matrix product
		m1 = is_matrix(L, 1);
		m2 = is_matrix(L, 2);

		// check if the matrices are the correct size
		luaL_argcheck(L, m1->cols == m2->rows, 2, "Wrong size");

		Matrix *m3 = make_matrix(L, m1->rows, m2->cols);

		for(int i = 0; i < m3->rows * m3->cols; i++) {
			m3->val[i] = 0;
			for(int j = 0; j < m1->cols; j++) {
				int m1_index = (i/m3->cols)*m1->cols + j;
				int m2_index = i%m3->cols + j*m3->cols;
				m3->val[i] += m1->val[ m1_index ] * m2->val[ m2_index ];
			}
		}
	}

	return 1;
}

/* Schur Product
 * 	Component-wise multiplication
 */
static int matrix_schur(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);

	Matrix *m3 = make_matrix(L, m1->rows, m1->cols);
	for(int i = 0; i < m1->rows*m1->cols; i++)
		m3->val[i] = m1->val[i] * m2->val[i];

	return 1;
}

static int matrix_map(lua_State * L) {
	Matrix *m = is_matrix(L, 1);
	luaL_argcheck(L, lua_isfunction(L, -1), 2, "`function' expected");
	
	Matrix *result = make_matrix(L, m->rows, m->cols); 

	for(int i = 0; i < m->rows * m->cols; i++) {
		lua_pushvalue(L, -2); 			// copy of function
		lua_pushnumber(L, m->val[i]); 		// copy of entry
		lua_pushinteger(L, get_row_from_index(i, m->cols));	// current row
		lua_pushinteger(L, get_col_from_index(i, m->cols));	// current column
		lua_call(L, 3, 1); 			// call the function, popping the function and argument off the stack
		luaL_argcheck(L, lua_isnumber(L, -1), 2,
		"function must return a number"); 	// check if result is a number
		result->val[i] = lua_tonumber(L, -1); 	// put it in the matrix
		lua_pop(L, 1); 				// pop the result off of the stack
	}

	return 1;
}
//}}}

//{{{ luaL_Reg and luaopen
// initialize the library
static const struct luaL_Reg matrixlib_funcs [] = {
	{"new", 	lua_make_matrix		},
	{"identity", 	make_identity_matrix	},
	{"random", 	make_random_matrix	},
	{NULL, 		NULL			}
};

// default methods for matrix object
static const struct luaL_Reg matrixlib_meta_index [] = {
	{"set",		lua_set_matrix_element	},
	{"get",		lua_get_matrix_element	},
	{"size",	lua_get_matrix_size	},
	{"rows", 	generate_rows		},
	{"cols", 	generate_cols		},
	{"entries", 	generate_entries	},
	{"map", 	matrix_map		},
	{"schur", 	matrix_schur		},
	{NULL, 		NULL			}
};

// arithmetic metamethods for matrix object
static const struct luaL_Reg matrixlib_metamethods [] = {
	{"__add", 	matrix_add		},
	{"__sub", 	matrix_sub		},
	{"__unm", 	matrix_unm		},
	{"__div", 	matrix_div		},
	{"__idiv", 	matrix_idiv		},
	{"__mod", 	matrix_mod		},
	{"__mul", 	matrix_mul		},
	{NULL, NULL}
};

// this basically get called when require("lua_matrix") is called
int luaopen_lua_matrix(lua_State *L) {
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
	return 1;				// return it to lua
}
//}}}



