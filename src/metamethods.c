#include "metamethods.h"

int matrix_add(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);
	Matrix *sum = make_matrix(L, m1->rows, m1->cols);
	arr_add(0, m1->rows * m1->cols, m1->val, m2->val, sum->val);
	return 1;
}

int matrix_sub(lua_State *L) {
	Matrix *m1 = is_matrix(L, 1);
	Matrix *m2 = is_matrix(L, 2);
	luaL_argcheck(L, same_size(m1, m2), 2, SIZE_ERR);
	Matrix *dif = make_matrix(L, m1->rows, m1->cols);
	arr_sub(0, m1->rows * m1->cols, m1->val, m2->val, dif->val);
	return 1;
}

int matrix_unm(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	Matrix *negm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++)
		negm->val[i] = -m->val[i];

	return 1;
}

int matrix_div(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double divisor = luaL_checknumber(L, 2);

	Matrix *newm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++) {
		newm->val[i] = m->val[i] / divisor;
	}

	return 1;
}

int matrix_idiv(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double divisor = luaL_checknumber(L, 2);

	Matrix *newm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++) {
		newm->val[i] = floor(m->val[i] / divisor);
	}

	return 1;
}

int matrix_mod(lua_State *L) {
	Matrix *m = is_matrix(L, 1);
	double divisor = luaL_checknumber(L, 2);

	Matrix *newm = make_matrix(L, m->rows, m->cols);
	for(int i = 0; i < m->rows * m->cols; i++) {
		newm->val[i] = fmod(m->val[i], divisor);
	}

	return 1;
}

int matrix_mul(lua_State *L) {
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
		multiply(
			m1->val, m1->rows, m1->cols,
			m2->val, m2->cols,
			m3->val
		);
	}

	return 1;
}

int matrix_pow(lua_State *L) {
	if(!lua_isinteger(L, -1))
		return luaL_argerror(L, 2, "Attempt to __pow matrix and non-integer");
	int num = lua_tointeger(L, -1);
	if(num <= 0)
		return luaL_argerror(L, 2, "Must be positive integer");

	Matrix *m = is_matrix(L, 1);
	luaL_argcheck(L, m->cols == m->rows, 1, "Matrix must be square");
	int size = m->rows * m->cols;

	double *temp = malloc(sizeof(double) * size);
	if(temp == NULL) 
		return luaL_error(L, "Unable to allocate memory buffer for matrix __pow.");

	Matrix *newMatrix = make_matrix(L, m->rows, m->cols);

	for(int i = 0; i < size; i++)
		newMatrix->val[i] = m->val[i];

	for(int i = 1; i < num; i++) {
		multiply(
			newMatrix->val, m->rows, m->cols,
			m->val, m->cols,
			temp
		);
		copy(0, size, temp, newMatrix->val);
	}

	free(temp);
	return 1;
}

int matrix_tostring(lua_State *L) {
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	Matrix *m = luaL_checkudata(L, 1, METATABLE);

	for(int r = 0; r < m->rows; r++) {
		for(int c = 0; c < m->cols; c++) {
			char buffer[12];
			snprintf(buffer, 12, "% .3e", m->val[get_index(m->cols, r+1, c+1)]);
			luaL_addstring(&b, buffer);
			if(c < m->cols - 1)
				luaL_addstring(&b, "\t");
		}
		if(r < m->rows - 1) luaL_addstring(&b, "\n");
	}

	luaL_pushresult(&b);
	return 1;
}

int matrix_gc(lua_State *L) {
	Matrix *m = luaL_checkudata(L, 1, METATABLE);
	free(m->val);
	return 0;
}

static int __newindex(lua_State *L) { // __newindex(self, key, value)
	luaL_argcheck(L, lua_isinteger(L, 2), 2, "Integer expected");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Number expected");

	Matrix *m = luaL_checkudata(L, lua_upvalueindex(1), METATABLE);
	int row = lua_tointeger(L, lua_upvalueindex(2));
	int col = lua_tointeger(L, 2);
	double value = lua_tonumber(L, 3);

	m->val[get_index(m->cols, row, col)] = value;
	return 0;
}

static int __index(lua_State *L) { // __index(self, key)
	luaL_argcheck(L, lua_isinteger(L, 2), 2, "Integer expected");

	Matrix *m = luaL_checkudata(L, lua_upvalueindex(1), METATABLE);
	int row = lua_tointeger(L, lua_upvalueindex(2));
	int col = lua_tointeger(L, 2);
	
	lua_pushnumber(L, m->val[get_index(m->cols, row, col)]);
	return 1;
}

static const luaL_Reg indexing_funcs[] = {
	{"__index", __index},
	{"__newindex", __newindex},
	{NULL, NULL}
};

static void create_metatable(lua_State *L, int row, int matrix_idx) {
	lua_newtable(L);
	lua_pushvalue(L, matrix_idx);
	lua_pushinteger(L, row);
	luaL_setfuncs(L, indexing_funcs, 2);
}

int matrix_index(lua_State *L) {
	if(lua_isinteger(L, 2)) {
		// return the row as a table with some __newindex and __index metamethods
		lua_newtable(L);
		create_metatable(L, lua_tointeger(L, 2), 1);
		lua_setmetatable(L, 3);

		return 1;
	} else if(lua_isstring(L, 2)) {
		// grab the method from the "methods" table in the metatable
		lua_getmetatable(L, 1); // 3
		lua_pushstring(L, "methods"); // 4
		lua_gettable(L, 3); // 4
		lua_rotate(L, -2, 1);
		lua_pop(L, 1); // 3
		lua_rotate(L, -2, 1);
		lua_gettable(L, 2);

		return 1;
	}

	lua_pushnil(L);
	return 1;
}

