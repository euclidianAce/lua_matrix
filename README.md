# lua\_matrix 
A matrix library for Lua, written in C. (This is basically a port of a library I've already written in Lua, but am using to learn the C api.)

# Todo:
#### Short Term
- [x] Basic Constructor
- [x] Basic Getters and Setters
- [x] Basic error checking for index out of range
- [ ] Either a \_\_tostring method or a simple print method to help debugging
- [ ] Add aritmetic metamethods
	- [x] \_\_add
	- [ ] \_\_sub
	- [ ] \_\_unm
	- [ ] \_\_mul (the important one)
		- [ ] Matrix Matrix product
		- [ ] Matrix Number product
	- [ ] \_\_div
	- [ ] \_\_idiv
	- [ ] \_\_mod (? probably mod each element)
	- [ ] \_\_pow (for integers)
- [ ] Add metamethods to allow access of matrices using index notation (i.e. a[2][3] instead of a:get(2,3))
- Different constructors for
	- [ ] Arbitrary Matrix initialized with a table of tables
	- [ ] Arbitrary Matrix initialized with a table of numbers and an int of rows
	- [ ] Identity Matrix
	- [ ] Random Matrix
- [ ] Iterators
	- [ ] rows - each call returns a row as a table
	- [ ] columns - each call returns a column as a table
	- [ ] entries - each call g
- [ ] Map function to apply function to each element of a matrix


#### Long Term
- [ ] Determinant method
- [ ] Row reduction and Gaussian Elimination methods
- [ ] Implement the Aguilera-Perez Algorithm for Rotation Matrix Constructor
- [ ] Maybe a Vector object?
	- [ ] With cross and dot products?

# Build (Linux)
Download the lua\_matrix.c file and with gcc run
```
$ gcc -shared -o lua_matrix.so -fPIC lua_matrix.c
```
This will produce a shared library (.so file) that when put in the same directory as a Lua file can just be required like so:
```
local matrix = require("lua_matrix")
```
