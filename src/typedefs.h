#ifndef TYPEDEF_H
#define TYPEDEF_H
// Unique metatable name for lua
#define METATABLE "lmatrix.Matrix"

typedef struct Matrix {
	int rows, cols;
	double *val;
} Matrix;

#endif
