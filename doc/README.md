# Constructors

- matrix.new(row:int, col:int)
	- returns a zero matrix with row rows and col columns
```lua
local a = matrix.new(2,2)
--[[ a => [0  0]
--	  [0  0]
--]]
```

- matrix.new(entries:table:number, rows:int)
	- returns a matrix initialized from the numbers in entries and divides it using rows
```lua
local a = matrix.new({1,2,3,4,5,6}, 2)
--[[ a => [1  2  3]
--	  [4  5  6]
--]]

local b = matrix.new({1,2,3,4,5,6}, 3)
--[[ b => [1  2]
--	  [3  4]
--	  [5  6]
--]]
```

- matrix.new(entries:table:table:number)
	- returns a matrix initialized from entries, which is a table of tables of numbers
	- The tables that are passed in are copied and not modified in any way
```lua
local a = matrix.new{{1,2,3,4},
		     {5,6,7,8}}
local b = matrix.new{{1,2},
		     {3,4},
		     {5,6},
		     {7,8}}
```

- matrix.identity(size:int)
	- returns a square identity matrix of the given size

# Arithmetic
All the standard arithmetic operators should work with matrices

- Addition, Subtraction
```lua
local a = matrix.new{{1,1},{1,1}}
local b = a + a --[[ => b = [2 2]
		--	    [2 2]
		--]]
local c = b - a --[[ => c = [1 1]
		--	    [1 1]
		--]]
```

- Matrix number, number Matrix, and Matrix Matrix multiplication
```lua
local a = matrix.new{{1,2},{3,4}}
local b = a * 2 --[[ => b = [2 4]
		--	    [6 8]
		--]]

local c = 0.5 * a --[[ => c = [0.5 1]
		  --	      [1.5 2]
		  --]]

local d = a * a   --[[ => d = [7  10]
		  --	      [15 22]
		  --]]
```
- Matrix number division
```lua
local a = matrix.new{{1,2},{3,4}}
local b = a / 2   --[[ => b = [0.5 1]
		  --	      [1.5 2]
		  --]]
```
- Matrix (positive) integer exponents
```lua
local a = matrix.new{{2,2},{2,2}}^3
--[[ => a = [8 8]
--	    [8 8]
--]]
```

# Matrix Object Metamethods
Each of these methods are part of the \_\_index table in the metatable and are called using the colon syntax
```lua
local m = matrix.new{{1,2,3},{4,5,6},{7,8,9}}
m:method(args)
```
- get(row:int, col:int) and set(row:int, col:int, num:number)
	- pretty self explanatory
```lua
local m = matrix.new(3,2)
m:set(2,1, 0.4)
m:get(2,1) -- => 0.4
m:get(1,1) -- => 0
```

- map( func(entry, row, col) )
	- returns a new matrix by passing each entry through the given function
	- also passes the row and column of the entry as the 2nd and 3rd arguments
	- func must return a number or map will error
```lua
-- to square each entry in the given matrix using map
local a = matrix.new{{1,2,3},
		     {4,5,6}}
a = a:map(function(e)
	return e^2
end)
--[[ =>
--	a = [ 1   4   9]
--	    [16  25  36]
--]]
```


# Iterators

```lua
for varlist in m:method() do
	-- stuff
end
```

- rows()
	- each iteration returns the row number and a table of the entries in that row
```lua
for rowIndex, rowTable in m:rows() do
	-- stuff
end
```
- columns()
	- each iteration returns the column number and a table of the entries in that column
```lua
for colIndex, colTable in m:cols() do
	-- stuff
end
```

- entries()
	- each iteration returns the row and column number and the entry in that position
```lua
for row, col, entry in m:entries() do
	-- stuff
end
```


