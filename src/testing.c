#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "lmatrix.h"


void console_log(const char* fmt, ...) {
	va_list args;
	printf("[C]: ");
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("No lua script given\n");
		return 0;
	}

	for(int i = 1; i < argc; i++) {
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
		console_log("Loading file \"%s\"", argv[i]);
		if(luaL_loadfile(L, argv[i]) != LUA_OK)
			console_log("Error loading file %s\n", argv[i]);
		else if(lua_pcall(L, 0, 0, 0) != LUA_OK) {
			console_log("Error in file %s\n", argv[i]);
			console_log("%s", lua_tostring(L, -1));
		}

		lua_close(L);
	}
	return 0;
}
