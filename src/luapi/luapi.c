/*
 * luapi.c
 */
#include <luapi.h>

static lua_State *L;
static void nspr_luapi_event_api(lua_State *L);

static void nspr_bail(lua_State *L, char *msg) {
    nspr_log_error("\nFATAL ERROR:\n  %s: %s\n\n", msg, lua_tostring(L, -1));
}

static int nspr_luapi_loadfile(lua_State *L) {
    if (luaL_loadfile(L, "work.lua")) {
        nspr_bail(L, "luaL_loadfile() failed");
        return NSPR_ERROR;
    }

    if (lua_pcall(L, 0, 0, 0)) {
        nspr_bail(L, "lua_pcall() failed");
        return NSPR_ERROR;
    }

    return NSPR_OK;
}

static void nspr_luapi_core_api(lua_State *L) {
    lua_pushliteral(L, "core");
    lua_newtable(L);    /*  .core table aka {} */

    lua_pushstring(L, "1.0.0");
    lua_setfield(L, -2, "version");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.core table */
}

#ifdef NSPR_HAVE_DEBUG
static int nspr_luapi_debug_arg0(lua_State *L) {
    lua_pushstring(L, "hello, nspider!");
    return 1;
}
static void nspr_luapi_debug_api(lua_State *L) {
    lua_pushliteral(L, "debug");
    lua_newtable(L);    /*  .debug table aka {} */

    lua_pushcfunction(L, nspr_luapi_debug_arg0);
    lua_setfield(L, -2, "debug_arg0");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.debug table */
}
#endif

static void nspr_luapi_nspider_api(lua_State *L) {
    lua_createtable(L, 0, 99);
#ifdef NSPR_HAVE_DEBUG
    nspr_luapi_debug_api(L);
#endif
    nspr_luapi_core_api(L);
    nspr_luapi_event_api(L);

    /*
     * loading package 
     */
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

// XXX luapi_event_api --> BEGIN
/*
 * event_api calling lua function from C
 */
static void luapi_event_api_read(nspr_event_node_fd_t *node)
{
    lua_getglobal(L, "onread");
    if (!lua_isfunction(L, -1)) {
        nspr_bail(L, "onread is not a function!");
        return;
    }
    lua_pushlightuserdata(L, node);
    if (lua_pcall(L, 1, 0, 0) != 0) {
        nspr_bail(L, "get default gw address failed");
        return;
    }
}

static void luapi_event_api_write(nspr_event_node_fd_t *node)
{
}

static void luapi_event_api_error(nspr_event_node_fd_t *node)
{
}

static int nspr_luapi_event_new(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_newuserdata(L, sizeof(nspr_event_node_fd_t));
    if (node == NULL) {
        return 0;
    }
    // we'll return the object
    return 1;
}

static int nspr_luapi_event_init(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_touserdata(L, 1);
    int fd = lua_tonumber(L, 2);
    int event_type = lua_tonumber(L, 3);
    node->fd = fd;
    node->event_type = event_type;
    node->read = luapi_event_api_read;
    node->write = luapi_event_api_write;
    node->error = luapi_event_api_error;
    return 0;
}

static int nspr_luapi_event_add(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_touserdata(L, 1);
    nspr_event_add(node);
    return 0;
}

static int nspr_luapi_event_del(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_topointer(L, 1);
    nspr_event_del(node);
    return 0;
}

static void nspr_luapi_event_api(lua_State *L) {
    lua_pushliteral(L, "event");
    lua_newtable(L);    /*  .event table aka {} */

    lua_pushcfunction(L, nspr_luapi_event_new);
    lua_setfield(L, -2, "new");
    lua_pushcfunction(L, nspr_luapi_event_init);
    lua_setfield(L, -2, "init");
    lua_pushcfunction(L, nspr_luapi_event_add);
    lua_setfield(L, -2, "add");
    lua_pushcfunction(L, nspr_luapi_event_del);
    lua_setfield(L, -2, "del");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.event table */
}

// XXX luapi_event_api --> END
