#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "lua_matrix.h"

#define SCRIPT "script.lua"


int main(int argc, char **argv) {
	if(argc != 2) {
		printf("No lua script given\n");
		return 0;
	}


	lua_State *L = luaL_newstate();
	if(!L) {
		printf("Unable to open lua state.\n"); 
		exit(1);
	}
	printf("[C]: Opened lua state\n");
	luaL_openlibs(L);

	lua_pushcfunction(L, luaopen_lua_matrix);
	lua_call(L, 0, 1);
	lua_setglobal(L, "matrix");

	int status = luaL_loadfile(L, argv[1]);
	if(status != LUA_OK)
		printf("Error loading file %s", argv[1]);
	else
		lua_call(L, 0, 0);

	lua_close(L);
	return 0;
}