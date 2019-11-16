# Constructors

- matrix.new(row, col)
	- returns a zero matrix with row rows and col columns
- matrix.new(entries, rows)
	- returns a matrix initialized from the numbers in entries and divides it using rows
- matrix.new(entries)
	- returns a matrix initialized from entries, which is a table of tables of numbers

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

# Iterators

```lua
for varlist in m:method() do
	-- stuff
end
```

- rows()
	- each iteration returns the row number and a table of the entries in that row
- columns()
	- each iteration returns the column number and a table of the entries in that column
- entries()
	- each iteration returns the row and column number and the entry in that position



