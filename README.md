# lua\_matrix 
A matrix library for Lua, written in C. (This is basically a port of a library I've already written in Lua, but am using to learn the C api.)


# Build (Linux)

### As a dynamic library
I do keep a binary in the bin directory as lua_matrix.so for easy access but if you'd like to build it yourself here's how.

#### Tup
I personally use the tup system to build things, so if you have tup installed you can do the following.
1. Clone the repo or curl/wget/whatever the src files
```
$ git clone git://github.com/3uclidian/lua_matrix.git
```
2. Initialize the tup repo inside and run tup
```
$ tup init
$ tup
```
This should compile everything, given that on your system you have:
- gcc
- The lua header files on your system

The lua\_matrix.so file should be placed in the bin directory

#### Manually
The Tupfile basically keeps track of the command so I don't have to remember how to compile a shared library every time but the command is here if you don't want to install tup.

1. Clone the repo or curl/wget/whatever the src files
2. run the following
```
$ gcc -shared -o lua_matrix.so -fPIC lua_matrix.c utils.c
```

# Use
Once you've either downoaded or compiled the library, either placing it in the directory of your Lua project or placing it in your LUA\_CPATH you can simply `require` it like any other library

```lua
local matrix = require "lua_matrix"
local a = matrix.new(3, 3)
```

For details of how to actually use the library, check out either the wiki or doc directory

