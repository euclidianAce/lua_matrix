#lua_matrix 
A matrix library for Lua, written in C. (This is basically a port of a library I've already written in Lua, but am using to learn the C api.)

#Todo:
#### Short Term
- [x] Basic Constructor
- [x] Basic Getters and Setters
- [ ] Add aritmetic metamethods
- [ ] Add metamethods to allow access of matrices using index notation (i.e. a[2][3] instead of a:get(2,3))
- [ ] Different constructors for
	- [ ] Arbitrary Matrix initialized with a table of tables
	- [ ] Identity Matrix
	- [ ] Random Matrix
- [ ] Map function to apply function to each element of a matrix


#### Long Term
- [ ] Determinant method
- [ ] Row reduction and Gaussian Elimination methods
- [ ] Implement the Aguilera-Perez Algorithm for Rotation Matrix Constructor

#Build (Linux)
Download the lua_matrix.c file and with gcc run
```
gcc -shared -o lua_matrix.so -fPIC lua_matrix.c
```
This will produce a shared library (.so file) that when put in the same directory as a Lua file can just be required like so:
```
local matrix = require("lua_matrix")
```
