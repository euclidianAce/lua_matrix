#include "vectormethods.h"

#define max(x, y) ((x < y) ? y : x)
#define vec_len(v_ptr) max(v_ptr->rows, v_ptr->cols)

// assuming v_ptr is definitely a vector
#define row_vec(v_ptr) (v_ptr->rows > v_ptr->cols)
#define col_vec(v_ptr) (v_ptr->cols > v_ptr->rows)

static Matrix *is_vector(lua_State *L, int index) {
	Matrix *m = is_matrix(L, index);
	luaL_argcheck(L, m->rows != 1 && m->cols != 1, index, "Vector expected");
	return m;
}

int vector_dot(lua_State *L) {
	Matrix *v1 = is_vector(L, 1);
	Matrix *v2 = is_vector(L, 2);
	luaL_argcheck(L, vec_len(v1) == vec_len(v2), 2, "Cannot dot vectors of different length");

	double result = 0;
	for(int i = 0; i < vec_len(v1); i++) {
		result += v1->val[i] * v2->val[i];
	}
	lua_pushnumber(L, result);
	return 1;
} 

int vector_cross(lua_State *L) {
	Matrix *v1 = is_vector(L, 1);
	Matrix *v2 = is_vector(L, 2);
	luaL_argcheck(L, vec_len(v1) == 3, 1, "Cannot cross vector of length other than 3");
	luaL_argcheck(L, vec_len(v2) == 3, 2, "Cannot cross vector of length other than 3");
	Matrix *result = make_matrix(L, v1->rows, v1->cols);
	result->val[0] = v1->val[1]*v2->val[2] - v1->val[2]*v2->val[1];
	result->val[1] = v1->val[2]*v2->val[0] - v1->val[0]*v2->val[2];
	result->val[2] = v1->val[0]*v2->val[1] - v1->val[1]*v2->val[0];

	return 1;
}

static double square_sum(double *arr, size_t size) {
	double sum = 0.0;
	for(int i = 0; i < size; i++) {
		sum += arr[i] * arr[i];
	}
	return sum;
}

int vector_magnitude_squared(lua_State *L) {
	Matrix *vec = is_vector(L, 1);
	lua_pushnumber(L, square_sum(vec->val, vec_len(vec)));
	return 1;
}

int vector_magnitude(lua_State *L) {
	Matrix *vec = is_vector(L, 1);
	lua_pushnumber(L, sqrt(square_sum(vec->val, vec_len(vec))));
	return 1;
}

int vector_normalize(lua_State *L) {
	Matrix *vec = is_vector(L, 1);
	double mag = sqrt(square_sum(vec->val, vec_len(vec)));
	if(mag == 0) {
		return luaL_argerror(L, 1, "Attempt to normalize vector of magnitude 0");
	}
	Matrix *new_vec = make_matrix(L, vec->rows, vec->cols);
	for(int i = 0; i < vec_len(vec); i++) {
		new_vec->val[i] = vec->val[i] / mag;
	}
	return 1;
}

