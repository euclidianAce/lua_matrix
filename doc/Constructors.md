```lua
local matrix = require "lmatrix"
```

# Basic Constructors

### `matrix.new(...)`
- Different types of arguments will produce different matrices
	- `rows:int, cols:int`
		Produces a zero matrix of the given size.
	- `entries:table, rows:int`
		Produces a matrix with the given entries divided into the given amount of rows.
	- `cols:int, entries:table`
		Produces a matrix with the given entries divided into the given amount of columns.

### `matrix.identity(size:int)`
Produces an identity matrix of the given size

### `matrix.random(rows:int, cols:int)`
Produces a matrix of the given size with entries in the range [-0.5, 0.5]

# Rotation and Translation Matrices

The implementation of rotation matrices is based on the [Aguilera-Perez algorithm.](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.4.8662)
In short, the algorithm describes a method of producing a general N-dimensional rotation matrix by composing main rotations and translations to turn the rotation about any axis into a main rotation, then composes all of those transformations.
Seeing as the algorithm is meant to handle any size of matrix and involves a lot of matrix multiplication, it winds up being (I think) O(n^3). So I would recommend pre-calculating any rotations you might use and store them instead of calculating them on-the-fly.

### `matrix.translation(vector:matrix)`
Produces a homogeneous translation matrix based on the given vector.
Will correctly handle row and column vectors.

### `matrix.mainRotation(size:int, axis1:int, axis2:int, angle:number)`
Produces a main rotation matrix of the given size about the given axes using the given angle.

### `matrix.rotation(simplex:matrix, angle:number)`
As described in the paper, the simplex is the generalization of the axis of rotation and will be a matrix with size of the form (n x n+1).
Produces a homogeneous rotation matrix about the given simplex of the given angle.

