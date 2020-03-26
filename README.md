# lmatrix 
A matrix library for Lua, written in C. (This is basically a port of a library I've already written in Lua, but am using to learn the C api.)


# Build (GNU make)

### As a dynamic library
I do keep a binary in the bin directory as lua\_matrix.so for easy access but if you'd like to build it yourself here's how.
Given that you have the lua headers on your system: `lua.h`, `lualib.h`, and `lauxlib.h`

1. clone the repo && cd into it
2. run make, and it (hopefully) should just work
```
$ make
```

The lmatrix.so file should be placed in the bin directory

#### Manually
The Makefile basically keeps track of the command so I don't have to remember how to compile a shared library every time but the command is here if you don't want to install tup.

1. Clone the repo or curl/wget/whatever the src files
2. run the following
```
$ gcc -shared -o lmatrix.so -fPIC *.c
```

# Use
Once you've either downloaded or compiled the library, either placing it in the directory of your Lua project or placing it in your LUA\_CPATH you can simply `require` it like any other library

```lua
local matrix = require "lmatrix"
local a = matrix.new(3, 3)
```

For details of how to actually use the library, check out either the wiki or doc directory

