#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "lmatrix.h"


void console_log(const char* str) {
	printf("[C]: %s\n", str);
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("No lua script given\n");
		return 0;
	} else if (argc > 2) {
		printf("Too many arguments.\n");
		return 0;
	}

	lua_State *L = luaL_newstate();
	if(!L) {
		console_log("Unable to open lua state."); 
		exit(1);
	}
	console_log("Opened lua state");
	luaL_openlibs(L);
	console_log("Opened standard library");
	lua_pushcfunction(L, luaopen_lmatrix);
	lua_call(L, 0, 1);
	lua_setglobal(L, "matrix");
	console_log("Opened matrix library");

	int status = luaL_loadfile(L, argv[1]);
	if(status != LUA_OK) 	printf("Error loading file %s\n", argv[1]);
	else 			lua_call(L, 0, 0);

	lua_close(L);
	return 0;
}
