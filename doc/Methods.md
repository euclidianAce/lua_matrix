```lua
local m = matrix.new(...)
```
# Getters and Setters
To change and get individual values in a matrix
### `m:get(row:int, col:int)`
Returns the value at the given position
### `m:set(row:int, col:int, entry:number)`
Sets the value at the given position

# Other
### `m:map( func(entry:number, row:int, col:int) )`
Produces a matrix by passing each entry through `func`. (func must return a number or map will error)

### `m:schur(n:matrix)`
Returns the [schur (or Hadamard) product](https://en.wikipedia.org/wiki/Hadamard_product_(matrices)) of `m` and `n`. (component-wise multiplication)
