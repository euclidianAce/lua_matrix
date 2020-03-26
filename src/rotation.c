/*
 * --------------------------------------------------------------------------------------------------
 * --   Implementation of the Aguilera-Perez Algorithm                                             --
 * --   Aguilera, Antonio, and Ricardo Perez-Aguilera. "General n-dimensional rotations." (2004)   --
 * --   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.4.8662                             --
 * --------------------------------------------------------------------------------------------------
 */

/* Basic structure of these constructors
 * void make_..._array
 * 	takes an array and populates it with the relevant values
 * Matrix make_..._matrix
 * 	creates the actual userdata, populates it with the previous function
 * int lua_...
 * 	the function that gets called from lua, validates arguments, the calls the previous function
 */
#include "rotation.h"

// Uses homogeneous matrices to translate and rotate
static void make_translation_array(int size, int row, double *values, double *result) {
	int i = size;
	int delta;
	if(row) {// row vector, elements go in the last row of matrix
		i *= (size+1); delta = 1;
	} else {// column vector, elements go in the last column of matrix
		delta = size+1;
	}
	make_identity_array(size, result);
	for(int j = 0; i < size*size-1; i+=delta, j++) {
		//printf("Writing to index %d of matrix of size %dx%d\n", i, size, size);
		result[i] = values[j];
	}
}

// takes a vector as an argument and produces a translation matrix
static Matrix *make_translation_matrix(lua_State *L, Matrix *vector) {
	int size = vector->rows == 1 ? vector->cols : vector->rows;
	int row = vector->rows == 1;
	Matrix *m = make_matrix(L, size+1, size+1);
	make_translation_array(size, row, vector->val, m->val);
	return m;
}

int lua_translation_matrix(lua_State *L) {
	// basically an identity matrix with the vector in the last row or column
	Matrix *vector = luaL_checkudata(L, 1, METATABLE);
	if(vector->rows != 1 && vector->cols != 1)
		luaL_argerror(L, 1, "Vector expected");
	int size = vector->rows;
	if(size == 1)
		size = vector->cols;
	make_translation_matrix(L, vector);
	
	return 1;
}

// a rotation about one of the main axes of some N-Dimensional space
static void main_rotation_array(int size, int axis1, int axis2, double angle, double *result) {
	make_identity_array(size, result);

	result[ get_index(size, axis1, axis1) ] = cos(angle);
	result[ get_index(size, axis2, axis2) ] = cos(angle);
	result[ get_index(size, axis1, axis2) ] = -sin(angle);
	result[ get_index(size, axis2, axis1) ] = sin(angle);
}
static Matrix *main_rotation_matrix(lua_State *L, int size, int axis1, int axis2, double angle) {
	Matrix *m = make_identity_matrix(L, size);
	main_rotation_array(size, axis1, axis2, angle, m->val);
	return m;
}
int lua_main_rotation_matrix(lua_State *L) {
	int size = luaL_checkinteger(L, 1);
	int axis1 = luaL_checkinteger(L, 2);
	int axis2 = luaL_checkinteger(L, 3);
	double angle = luaL_checknumber(L, 4);
	main_rotation_matrix(L, size, axis1, axis2, angle);

	return 1;
}

// a general rotation composed of translations and main rotations,
// given a simplex to rotate about and an angle of rotation
static void rotation_array(lua_State *L, Matrix *v0 /* simplex */, double angle, double *M) {
	int n = v0->cols;
	int size = (n+1)*(n+1);

	/* Memory to be allocated
	 * M: to accumulate the rotations + translation
	 * M_inv: to accumulate the inverse of rotations + translation
	 * Mk: a temp value used inside the main loop of the algorithm to compute interstitial values of M and M_inv
	 * v: to accumulate how the simplex changes as rotations are applied
	 */

	double *M_inv, *Mk, *v, *temp_M, *temp_vector;
	size_t bytes = sizeof(double);
	v = calloc(v0->rows * (n+1), bytes);
	M_inv = calloc(size, bytes);
	Mk = calloc(size, bytes);
	temp_M = calloc(size, bytes);
	temp_vector = calloc(n, bytes);
	if(M_inv==NULL || v==NULL || Mk==NULL || temp_M==NULL || temp_vector==NULL ) {
		free(M_inv);free(v);free(Mk);free(temp_M);free(temp_vector);
		ALLOC_FAIL(L);
	}

	/* 		Initialization		  	*/
	// copy v0 into v, except the first row, which gets turned to 0s
	// and v is composed of homogeneous vectors, so make the last column 1s
	for(int i = v0->cols; i < v0->rows * v0->cols; i += v0->cols) {
		copy(i, i + v0->cols, v0->val, v);
		v[i + v0->cols] = 1.0;
	}

	// initial values for M and M_inv as the initia translation and inverse of that
	copy(0, v0->cols, v0->val, temp_vector);
	make_translation_array(n+1, 1, temp_vector, M);

	for(int i = 0; i < v0->cols; i++) temp_vector[i] = -temp_vector[i];
	make_translation_array(n+1, 1, temp_vector, M_inv);

	/*		The Algorithm Itself		*/
	for(int r = 2; r < n - 1; r++) {
		for(int c = n; c >= r; c--) {
			double theta = atan2( v[ get_index(v0->rows, r, c) ], v[ get_index(v0->rows, r, c-1) ] );

			// Mk = R_c,c-1 (theta)
			main_rotation_array(n+1, c, c-1, theta, Mk);

			// v = v * Mk: This just takes the element in row r and col c of v and sets it to zero
			// No need for an actual multiply
			v[ get_index(n+1, r, c) ] = 0;

			// M = M * Mk
			multiply(
				M, n+1, n+1,
				Mk, n+1,
				temp_M
			);
			copy(0, size, temp_M, M);
			
			// Inverse
			// the inverse rotation has the same elements but with the sin(theta) terms negated
			Mk[ get_index(n+1, r, c) ] *= -1.0;
			Mk[ get_index(n+1, c, r) ] *= -1.0;
			
			multiply(
				Mk, n+1, n+1,
				M_inv, n+1,
				temp_M
			);
			copy(0, size, temp_M, M_inv);
		}
	}

	// M = M * R_n-1,n * M_inv
	main_rotation_array(n+1, n-1, n, angle, Mk);
	multiply(
		M, n+1, n+1,
		Mk, n+1,
		temp_M
	);
	multiply(
		temp_M, n+1, n+1,
		M_inv, n+1,
		M
	);
	free(temp_vector);free(v);free(M_inv);free(Mk);free(temp_M);
}

static Matrix *rotation_matrix(lua_State *L, Matrix *simplex, double angle) {
	int n = simplex->cols+1;
	Matrix *m = make_matrix(L, n, n);
	rotation_array(L, simplex, angle, m->val);
	return m;
}

int lua_rotation_matrix(lua_State *L) {
	/* args: (simplex:Matrix, angle:number)
	 *	- simplex
	 *		- must be of a certain size (n x n+1)
	 *
	 *	- angle
	 *		- just a number
	 */
	
	Matrix *simplex = luaL_checkudata(L, 1, METATABLE);
	luaL_argcheck(L, simplex->rows + 1 == simplex->cols, 1, "Simplex size must be of the form (N x N+1)");

	double angle = luaL_checknumber(L, 2);

	rotation_matrix(L, simplex, angle);

	return 1;
}

