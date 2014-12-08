/*
 * =====================================================================================
 *
 *       Filename:  hello.c
 *
 *    Description:  hello calling lua script.
 *
 *        Version:  1.0
 *        Created:  2012年11月03日 11时50分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */

#include <stdlib.h>                             /* For function exit() */
#include <stdio.h>                              /* For input/output */
#include <math.h>

static int l_sin(lua_State *L) {
        double d = luaL_checknumber(L, 1);
        lua_pushnumber(L, sin(d));
        return 1;
}

void bail(lua_State *L, char *msg){
        fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
                        msg, lua_tostring(L, -1));
        exit(1);
}

int main(void)
{
        lua_State *L;

        L = luaL_newstate();                        /* Create Lua state variable */
        luaL_openlibs(L);                           /* Load Lua libraries */


	// create a global tables
	lua_createtable(L, 0, 1);

	lua_pushboolean(L, 1);
	lua_setfield(L, -2, "debug");

        lua_pushcfunction(L, l_sin);
        lua_setfield(L, -2, "mysin");

	lua_getglobal(L, "package"); /* ngx package */
	lua_getfield(L, -1, "loaded"); /* ngx package loaded */
	lua_pushvalue(L, -3); /* ngx package loaded ngx */
	lua_setfield(L, -2, "ngx"); /* ngx package loaded */
	lua_pop(L, 2);
	lua_setglobal(L, "ngx");


        if (luaL_loadfile(L, "test.lua"))    /* Load but don't run the Lua script */
                bail(L, "luaL_loadfile() failed");      /* Error out if file can't be read */

        printf("In C, calling Lua\n");

        if (lua_pcall(L, 0, 0, 0))                  /* Run the loaded Lua script */
                bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

        printf("Back in C again\n");

        lua_close(L);                               /* Clean up, free the Lua state var */

        return 0;
}
