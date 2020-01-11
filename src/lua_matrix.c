// a matrix library for Lua 5.3.5

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "typedefs.h"
#include "utils.h"
#include "constructors.h"
#include "rotation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE_ERR "Matrices not same size"

//{{{ Constructors

static int lua_make_matrix(lua_State *L) {
	// These ifs basically "overload" this function without actually overloading it
	// since it can only take the lua_State as an arg
	
	// throw out any arguments past the first 2
	lua_settop(L, 2);

	// (int, int)
	if(lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
		int rows, cols;
		rows = luaL_checkinteger(L, 1);
		cols = luaL_checkinteger(L, 2);
		make_matrix(L, rows, cols);
		return 1;
	}

	// (int, table) or (table, int)
	if( (lua_istable(L, 1) && lua_isinteger(L, 2)) 
	||  (lua_istable(L, 2) && lua_isinteger(L, 1)) ) 
	{
		int arg1, arg2; // arg1: rows or columns, arg2: length of table
		Matrix *m;
		if(lua_isinteger(L, 1)) { // # of columns
			arg1 = lua_tointeger(L, 1);
			arg2 = luaL_len(L, 2);
			m = make_matrix(L, arg1, arg2 / arg1);
		} else { // # of rows
			arg1 = lua_tointeger(L, 2);
			arg2 = luaL_len(L, 1);
			lua_rotate(L, 1, 1); // guarantee that the table is on the top of the stack
			m = make_matrix(L, arg2 / arg1, arg1);
		}
		if(arg2 % arg1 != 0) {
			free(m->val);
			return luaL_argerror(L, 2, "Table not evenly divisible");
		}

		for(int i = 0; i < arg2; i++) {
			if(lua_geti(L, 2, i+1) != LUA_TNUMBER) {
				free(m->val);
				return luaL_argerror(L, 2, "Bad table value, number expected");
			}
			m->val[i] = lua_tonumber(L, -1);
			lua_pop(L, 1);
		}

		return 1;
	}
	
	// (table)
	if(lua_istable(L, 1)) {
		lua_settop(L, 1);
		int rows, cols;
		rows = luaL_len(L, 1);

		lua_geti(L, 1, 1);
		if(!lua_istable(L, -1))
			return luaL_argerror(L, 1, "Bad table value, table expected"); 
		cols = luaL_len(L, -1);
		lua_pop(L, 1);
		
		Matrix *m = make_matrix(L, rows, cols);
		for(int i = 0; i < rows; i++) {
			lua_geti(L, -2, i+1);
			if(luaL_len(L, -1) != cols) {
				free(m->val);
				return luaL_argerror(L, 1, "Tables must all be same length"); 
			}

			for(int j = 0; j < cols; j++) {
				lua_geti(L, -1, j+1);
				if(!lua_isnumber(L, -1)) {
					free(m->val);
					return luaL_argerror(L, 1, "Bad table value, number expected"); 
				}
				m->val[i*cols + j] = lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}

		return 1;
	}

	return luaL_argerror(L, 1, "Table or int expected");
}

static int lua_make_identity_matrix(lua_State *L) {
	int size = luaL_checkinteger(L, 1);
	make_identity_matrix(L, size);
	return 1;
}

static int make_random_matrix(lua_State *L) {
	int rows = luaL_checkinteger(L, 1);
	int cols = luaL_checkinteger(L, 2);
	Matrix *m = make_matrix(L, rows, cols);
	for(int i = 0; i < rows*cols; i++)
		m->val[i] = (double) rand()/RAND_MAX - 0.5;
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
	is_matrix(L, 1);		// validate that the first argument is a matrix
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
	is_matrix(L, 1);		// validate that the top of the stack is a matrix
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
	is_matrix(L, 1);		// validate that the top of the stack is a matrix
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
	Matrix *m1, *m2;

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
		multiply(m1->val, m1->rows, m1->cols,
			 m2->val, m2->cols,
			 m3->val);
	}

	return 1;
}

static int matrix_pow(lua_State *L) {
	// check that the second arg is a number
	if(!lua_isinteger(L, -1))
		return luaL_argerror(L, 2, "Attempt to __pow matrix and non-integer");
	int num = lua_tointeger(L, -1);
	// check if number is positive
	if(num <= 0)
		return luaL_argerror(L, 2, "Must be positive integer");

	Matrix *m = is_matrix(L, 1);
	// check if matrix is square
	luaL_argcheck(L, m->cols == m->rows, 1, "Matrix must be square");
	int size = m->rows * m->cols;

	double *temp = calloc(size, sizeof(double));
	if(temp == NULL) 
		return luaL_error(L, "Unable to allocate memory buffer for matrix __pow.");

	Matrix *newMatrix = make_matrix(L, m->rows, m->cols);

	for(int i = 0; i < size; i++)
		newMatrix->val[i] = m->val[i];

	for(int i = 1; i < num; i++) {
		// perform the multiplication in temp
		multiply(newMatrix->val, m->rows, m->cols,
			 m->val, m->cols,
			 temp);
		// copy temp into newMatrix
		for(int k = 0; k < size; k++)
			newMatrix->val[k] = temp[k];
		// rinse and repeat
	}

	free(temp);
	return 1;
}

// Garbage collection; frees the memory used to store the values
static int matrix_gc(lua_State *L) {
	Matrix *m = luaL_checkudata(L, 1, METATABLE);
	free(m->val);
	return 0;
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
	{"identity", 	lua_make_identity_matrix},
	{"random", 	make_random_matrix	},
	{"translation",	lua_translation_matrix	},
	{"mainRotation",lua_main_rotation_matrix},
	{"rotation",	lua_rotation_matrix	},
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
	{"__pow",	matrix_pow		},
	{"__gc", 	matrix_gc		},
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

