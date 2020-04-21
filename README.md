# lmatrix 
A matrix library for Lua, written in C. (This is basically a port of a library I've already written in Lua, but am using to learn the C api.)


# Build (GNU make)

### As a dynamic library
Given that you have the lua headers on your system: `lua.h`, `lualib.h`, and `lauxlib.h`

1. clone the repo && cd into it
2. run make, and it (hopefully) should just work
```
$ make
```

Additionally, I keep a simple test suite. You can run
```
$ make testing
$ cd testing/
$ ./runTests noErrors.lua
```
to build and run the test suite.

#### Manually

1. Clone the repo or curl/wget/whatever the src files
2. run the following
```
$ gcc -shared -o lmatrix.so -fPIC src/*.c
```

# Use
Once you've either downloaded or compiled the library, either placing it in the directory of your Lua project or placing it in your LUA\_CPATH you can simply `require` it like any other library

```lua
local matrix = require "lmatrix"
local a = matrix.new{
	{1,2,3},
	{4,5,6},
	{7,8,9}
}
local b = matrix.random(3, 3)
print(a*b)
```

For details of how to actually use the library, check out either the wiki or doc directory
Currently the docs are a little outdated, hopefully I'll get to it soon.

