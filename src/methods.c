#include "methods.h"

int lua_get_matrix_size(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	lua_pushinteger(L, m->rows);
	lua_pushinteger(L, m->cols);
	return 2;
}

/* Schur Product
 * 	Component-wise multiplication
 */
int matrix_schur(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);

	Matrix *m3 = make_matrix(L, m1->rows, m1->cols);
	for(int i = 0; i < m1->rows*m1->cols; i++)
		m3->val[i] = m1->val[i] * m2->val[i];

	return 1;
}

int matrix_map(lua_State * L) {
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

// Mutating arithmetic methods

int matrix_mut_add(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);
	arr_add(0, m1->rows * m1->cols, m1->val, m2->val, m1->val);
	return 0;
}
int matrix_mut_sub(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);
	arr_sub(0, m1->rows * m1->cols, m1->val, m2->val, m1->val);
	return 0;
}
int matrix_mut_unm(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	arr_unm(0, m1->rows * m1->cols, m1->val, m1->val);
	return 0;
}
int matrix_mut_div(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double d = luaL_checknumber(L, 2);
	arr_div(0, m->rows * m->cols, m->val, d, m->val);
	return 0;
}
int matrix_mut_idiv(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double d = luaL_checknumber(L, 2);
	arr_idiv(0, m->rows * m->cols, m->val, d, m->val);
	return 0;
}


