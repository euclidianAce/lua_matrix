# Constructors

- matrix.new(row, col)
	- returns a zero matrix with row rows and col columns
- matrix.new(entries, rows)
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

- matrix.new(entries)
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

- matrix.identity(int size)
	- returns a square identity matrix of the given size

# Matrix Object Metamethods
Each of these methods are part of the \_\_index table in the metatable and are called using the colon syntax
```lua
m = matrix.new{{1,2,3},{4,5,6},{7,8,9}}
m:method(args)
```
- get(row, col) and set(row, col, num)
	- pretty self explanatory

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


