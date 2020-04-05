
The vector table is stored in the matrix table like so
```lua
local vector = require("lmatrix").vector
```
Most of these are just for convenience. Vectors are not a different object, but just matrices with either a single row or column. Therefore any matrix methods will work with vectors as well (given that they are well defined).

# Constructors

### `vector.newRow(...)`
- Different arguments will produce different results
	- `cols:int`
		Produces a zero vector with the given number of columns.
	- `entries:table`
		Produces a vector initialized with the values in the given table.


### `vector.newCol(...)`
- Different arguments will produce different results
	- `rows:int`
		Produces a zero vector with the given number of rows.
	- `entries:table`
		Produces a vector initialized with the values in the given table.

# Methods
### `vector:cross`
### `vector:dot`

