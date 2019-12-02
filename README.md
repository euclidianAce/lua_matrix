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
	- [x] \_\_sub
	- [x] \_\_unm
	- [x] \_\_mul (the important one)
		- [x] Matrix Number product
		- [x] Matrix Matrix product
	- [x] \_\_div
	- [x] \_\_idiv
	- [x] \_\_mod (? probably mod each element)
	- [ ] \_\_pow (for integers)
- [ ] Add metamethods to allow access of matrices using index notation (i.e. a[2][3] instead of a:get(2,3))
	- This might be either impossible to do with only C or painstakingly hard
- Different constructors for
	- [x] Arbitrary Matrix initialized with a table of tables
	- [x] Arbitrary Matrix initialized with a table of numbers and an int of rows
	- [x] Identity Matrix of variable size
	- [x] Random Matrix
- [x] Iterators
	- [x] rows - each call returns a row as a table
	- [x] columns - each call returns a column
	- [x] entries - each call returns a single element
- [x] Map function to apply function to each element of a matrix
- [x] Schur product metamethod (component-wise multiplication, useful for neural networks and such)
	- Even though this is almost trivially implementable, idk what to call it because ```m:schur(n)``` seems a little weird
- [ ] Better Error messages

#### Mid Term
- [ ] Learn to use header files and if they might be needed for this project.
- [ ] Possibly redefine how the Matrix typedef struct works.
- [ ] Separate Vector Class?
	- [ ] Something Something Polymorphism

#### Long Term
- [ ] Determinant method
- [ ] Row reduction and Gaussian Elimination methods
- [ ] Implement the Aguilera-Perez Algorithm for Rotation Matrix Constructor
- [ ] Maybe a Vector object?
	- [ ] With cross and dot products?

# Build (Linux)

#### As a dynamic library
Download the lua\_matrix.c file and move it to the lua-5.3.5/src directory (or a directory with the appropriate header files). Then with gcc run
```
$ gcc -shared -o lua_matrix.so -fPIC lua_matrix.c
```
This will produce a shared library (.so file) that when put in the same directory as a Lua file can just be required like so:
```
local matrix = require("lua_matrix")
```

Or you could clone the repo (and add a copy of the lua 5.3.5 src to the place indicated in lua_matrix.c) and do a 
```
make
```
command while in the directory and the makefile should just execute the above command

#### Compiling directly into lua
There are plenty of guides elsewhere on the internet to compile a library into lua and I can't really be bothered to do this section right now.

