#ifndef TYPEDEF_H
#define TYPEDEF_H

#define METATABLE "lmatrix.Matrix"

typedef struct Matrix {
	int rows, cols;
	double *val;
} Matrix;

#endif
