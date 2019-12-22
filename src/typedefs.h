
#ifndef _TYPEDEF_H
#define _TYPEDEF_H
#define METATABLE "lua_matrix.Matrix"

typedef struct Matrix {
	int rows, cols;
	double val[1];
} Matrix;
#endif
