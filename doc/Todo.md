# Todo:
#### Short Term
- [x] Basic Constructor
- [x] Basic Getters and Setters
- [x] Basic error checking for index out of range
- [ ] Either a \_\_tostring method or a simple print method to help debugging
- [x] Add aritmetic metamethods
	- [x] \_\_add
	- [x] \_\_sub
	- [x] \_\_unm
	- [x] \_\_mul (the important one)
		- [x] Matrix Number product
		- [x] Matrix Matrix product
	- [x] \_\_div
	- [x] \_\_idiv
	- [x] \_\_mod (? probably mod each element)
	- [x] \_\_pow (for integers at least)
- Different constructors for
	- [x] Arbitrary Matrix initialized with a table of tables
	- [x] Arbitrary Matrix initialized with a table of numbers and an int of rows
	- [x] Arbitrary Matrix initialized with an int of columns and table of numbers
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
- [ ] Better monitor memory allocation and throw errors if it fails

#### Mid Term
- [x] Learn to use header files and if they might be needed for this project.
- [x] Possibly redefine how the Matrix typedef struct works.
- [ ] Separate Vector Class?
	- [ ] Something Something Polymorphism
	- [ ] Cross and dot products

#### Long Term
- [ ] Determinant method
- [ ] Row reduction and Gaussian Elimination methods
- [ ] Implement the Aguilera-Perez Algorithm for Rotation Matrix Constructor


