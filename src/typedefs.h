#ifndef _TYPEDEF_H
#define _TYPEDEF_H
// Unique metatable name for lua
#define METATABLE "lmatrix.Matrix"

typedef struct Matrix {
	int rows, cols;
	double *val;
} Matrix;

#endif
