#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include "utils.h"
#include "typedefs.h"


// allocate memory, set metatable, put pointer on stack
Matrix *make_matrix(lua_State *L, int rows, int cols) {
	size_t n = sizeof(Matrix);
	Matrix *m = (Matrix *)lua_newuserdata(L, n);
	m->rows = rows;
	m->cols = cols;
	m->val = calloc(rows * cols, sizeof(double));

	if(m->val == NULL) ALLOC_FAIL(L);
	
	luaL_setmetatable(L, METATABLE);
	return m;
}


void make_identity_array(int size, double *result) {
	for(int i = 0; i < size*size; i+=size+1)
		result[i] = 1;
}

Matrix *make_identity_matrix(lua_State *L, int size) {
	Matrix *m = make_matrix(L, size, size);
	make_identity_array(size, m->val);
	return m;
}
