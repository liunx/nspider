/*
 * luapi.c
 */
#include <luapi.h>

static lua_State *L;
static int nspr_luapi_event_add(lua_State *L);

static void bail(lua_State *L, char *msg) {
    nspr_log_error("\nFATAL ERROR:\n  %s: %s\n\n", msg, lua_tostring(L, -1));
}

static int nspr_luapi_loadfile(lua_State *L) {
    if (luaL_loadfile(L, "work.lua")) {
        bail(L, "luaL_loadfile() failed");
        return NSPR_ERROR;
    }

    if (lua_pcall(L, 0, 0, 0)) {
        bail(L, "lua_pcall() failed");
        return NSPR_ERROR;
    }

    return NSPR_OK;
}

static void nspr_luapi_core_api(lua_State *L) {
    lua_pushstring(L, "1.0.0");
    lua_setfield(L, -2, "version");
}

static void nspr_luapi_event_api(lua_State *L) {
    lua_pushcfunction(L, nspr_luapi_event_add);
    lua_setfield(L, -2, "event_add");
}

#ifdef NSPR_HAVE_DEBUG
static int nspr_luapi_debug_arg0(lua_State *L) {
    lua_pushstring(L, "hello, nspider!");
    return 1;
}
static void nspr_luapi_debug_api(lua_State *L) {
    lua_pushcfunction(L, nspr_luapi_debug_arg0);
    lua_setfield(L, -2, "debug_arg0");
}
#endif

static void nspr_luapi_nspider_api(lua_State *L) {
    lua_createtable(L, 0, 99);
#ifdef NSPR_HAVE_DEBUG
    nspr_luapi_debug_api(L);
#endif
    nspr_luapi_core_api(L);
    nspr_luapi_event_api(L);
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "loaded");
    lua_pushvalue(L, -3);
    lua_setfield(L, -2, "nspr");
    lua_pop(L, 2);
    lua_setglobal(L, "nspr");
}

int nspr_luapi_init(void) {
    int ret = NSPR_ERROR;
    L = luaL_newstate();
    luaL_openlibs(L);
    nspr_luapi_nspider_api(L);
    if ((ret = nspr_luapi_loadfile(L)) != NSPR_OK)
        return ret;

    return NSPR_OK;
}

void nspr_luapi_exit(void) {
    lua_close(L);
}

nspr_nspider_t nspr_luapi = {
    .id =  NSPR_ID_LUAPI,
    .init = nspr_luapi_init,
    .exit = nspr_luapi_exit,
};

static int nspr_luapi_event_add(lua_State *L) {
    return 1;
}
