#include "constructors.h"

Matrix *make_matrix(lua_State *L, int rows, int cols) {
	Matrix *m = (Matrix *)lua_newuserdata(L, sizeof(Matrix));
	m->rows = rows;
	m->cols = cols;
	m->val = calloc(rows * cols, sizeof(double));

	luaL_setmetatable(L, METATABLE);
	return m; 
}


void make_identity_array(int size, double *result) {
	for(int i = 0; i < size * size; i++)
		result[i] = get_row_from_index(i, size) == get_col_from_index(i, size) ? 1 : 0;
}

Matrix *make_identity_matrix(lua_State *L, int size) {
	Matrix *m = make_matrix(L, size, size);
	make_identity_array(size, m->val);
	return m;
}

int lua_make_matrix(lua_State *L) {
	// These ifs basically "overload" this function without actually overloading it
	// since it can only take the lua_State as an arg

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
		//lua_settop(L, 1);
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

int lua_make_identity_matrix(lua_State *L) {
	int size = luaL_checkinteger(L, 1);
	make_identity_matrix(L, size);
	return 1;
}

int make_random_matrix(lua_State *L) {
	int rows = luaL_checkinteger(L, 1);
	int cols = luaL_checkinteger(L, 2);
	Matrix *m = make_matrix(L, rows, cols);
	for(int i = 0; i < rows*cols; i++)
		m->val[i] = (double) rand()/RAND_MAX - 0.5;
	return 1;
}

