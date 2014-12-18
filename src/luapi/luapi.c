/*
 * luapi.c
 */
#include <luapi.h>

static lua_State *L;
static void nspr_luapi_event_api(lua_State *L);
static void nspr_luapi_inet_api(lua_State *L);
static void nspr_luapi_file_api(lua_State *L);
static void nspr_luapi_timer_api(lua_State *L);
static void nspr_luapi_signal_api(lua_State *L);

static void nspr_bail(lua_State *L, char *msg) {
    nspr_log_error("\nFATAL ERROR:\n  %s: %s\n\n", msg, lua_tostring(L, -1));
}

static int nspr_luapi_loadfile(lua_State *L) {
    if (luaL_loadfile(L, (char *)nspr_conf_file)) {
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
    lua_pushnumber(L, NSPR_OK);
    lua_setfield(L, -2, "NSPR_OK");
    lua_pushnumber(L, NSPR_ERROR);
    lua_setfield(L, -2, "NSPR_ERROR");

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
    nspr_luapi_inet_api(L);
    nspr_luapi_file_api(L);
    nspr_luapi_timer_api(L);
    nspr_luapi_signal_api(L);

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
static void nspr_luapi_event_api_read(nspr_event_node_fd_t *node)
{
    lua_getglobal(L, "event_read");
    if (!lua_isfunction(L, -1)) {
        nspr_bail(L, "event_read is not a function!");
        return;
    }
    lua_pushlightuserdata(L, node);
    if (lua_pcall(L, 1, 0, 0) != 0) {
        return;
    }
}

static void nspr_luapi_event_api_write(nspr_event_node_fd_t *node)
{
    lua_getglobal(L, "event_write");
    if (!lua_isfunction(L, -1)) {
        nspr_bail(L, "event_write is not a function!");
        return;
    }
    lua_pushlightuserdata(L, node);
    if (lua_pcall(L, 1, 0, 0) != 0) {
        return;
    }
}

static void nspr_luapi_event_api_error(nspr_event_node_fd_t *node)
{
    lua_getglobal(L, "event_error");
    if (!lua_isfunction(L, -1)) {
        nspr_bail(L, "event_error is not a function!");
        return;
    }
    lua_pushlightuserdata(L, node);
    if (lua_pcall(L, 1, 0, 0) != 0) {
        return;
    }
}

static void nspr_luapi_event_api_timer(nspr_event_timer_t *timer)
{
    lua_getglobal(L, "event_timer");
    if (!lua_isfunction(L, -1)) {
        nspr_bail(L, "event_timer is not a function!");
        return;
    }
    lua_pushlightuserdata(L, timer);
    if (lua_pcall(L, 1, 0, 0) != 0) {
        return;
    }
}

static void nspr_luapi_event_api_signal(nspr_event_signal_t *signal)
{
    lua_getglobal(L, "event_signal");
    if (!lua_isfunction(L, -1)) {
        nspr_bail(L, "event_signal is not a function!");
        return;
    }
    lua_pushlightuserdata(L, signal);
    if (lua_pcall(L, 1, 0, 0) != 0) {
        return;
    }
}

static int nspr_luapi_event_api_utils_getfd(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_topointer(L, 1);
    lua_pushnumber(L, node->fd);
    return 1;
}

static int nspr_luapi_event_api_utils_getid(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_topointer(L, 1);
    lua_pushnumber(L, node->eventid);
    return 1;
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
    unsigned int eventid = lua_tonumber(L, 4);
    node->fd = fd;
    node->event_type = event_type;
    node->eventid = eventid;
    node->read = nspr_luapi_event_api_read;
    node->write = nspr_luapi_event_api_write;
    node->error = nspr_luapi_event_api_error;
    return 0;
}

static int nspr_luapi_event_add(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_touserdata(L, 1);
    int event_type = lua_tonumber(L, 2);
    node->event_type = event_type;
    nspr_event_add(node);
    return 0;
}

static int nspr_luapi_event_del(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_touserdata(L, 1);
    nspr_event_del(node);
    return 0;
}

static int nspr_luapi_event_break(lua_State *L) {
    nspr_event_break();
    return 0;
}

static void nspr_luapi_event_api(lua_State *L) {
    lua_pushliteral(L, "event");
    lua_newtable(L);    /*  .event table aka {} */

    // add event constants
    lua_pushnumber(L, NSPR_EVENT_TYPE_READ);
    lua_setfield(L, -2, "NSPR_EVENT_TYPE_READ");
    lua_pushnumber(L, NSPR_EVENT_TYPE_WRITE);
    lua_setfield(L, -2, "NSPR_EVENT_TYPE_WRITE");
    lua_pushnumber(L, NSPR_EVENT_TYPE_ERROR);
    lua_setfield(L, -2, "NSPR_EVENT_TYPE_ERROR");
    lua_pushnumber(L, NSPR_EVENT_TYPE_TIMER);
    lua_setfield(L, -2, "NSPR_EVENT_TYPE_TIMER");
    lua_pushnumber(L, NSPR_EVENT_TYPE_SIGNAL);
    lua_setfield(L, -2, "NSPR_EVENT_TYPE_SIGNAL");

    lua_pushcfunction(L, nspr_luapi_event_api_utils_getfd);
    lua_setfield(L, -2, "getfd");
    lua_pushcfunction(L, nspr_luapi_event_api_utils_getid);
    lua_setfield(L, -2, "getid");

    lua_pushcfunction(L, nspr_luapi_event_new);
    lua_setfield(L, -2, "new");
    lua_pushcfunction(L, nspr_luapi_event_init);
    lua_setfield(L, -2, "init");
    lua_pushcfunction(L, nspr_luapi_event_add);
    lua_setfield(L, -2, "add");
    lua_pushcfunction(L, nspr_luapi_event_del);
    lua_setfield(L, -2, "del");
    lua_pushcfunction(L, nspr_luapi_event_break);
    lua_setfield(L, -2, "exit");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.event table */
}

// XXX luapi_event_api --> END

// XXX luapi_inet_api --> BEGIN
static int nspr_luapi_inet_api_listen(lua_State *L) {
    const char *address = lua_tostring(L, 1);
    int port = lua_tonumber(L, 2);
    int fd = nspr_create_ipv4_tcp_listen(address, port);
    lua_pushnumber(L, fd);
    return 1;
}

static int nspr_luapi_inet_api_accept(lua_State *L) {
    int listen_fd = lua_tonumber(L, 1);
    int fd = nspr_create_ipv4_tcp_accept(listen_fd);
    lua_pushnumber(L, fd);
    return 1;
}

static int nspr_luapi_inet_api_connect(lua_State *L) {
    const char *address = lua_tostring(L, 1);
    int port = lua_tonumber(L, 2);
    int fd = nspr_create_ipv4_tcp_connect(address, port);
    lua_pushnumber(L, fd);
    return 1;
}

static void nspr_luapi_inet_api(lua_State *L) {
    lua_pushliteral(L, "inet");
    lua_newtable(L);    /*  .inet table aka {} */

    lua_pushcfunction(L, nspr_luapi_inet_api_listen);
    lua_setfield(L, -2, "listen");
    lua_pushcfunction(L, nspr_luapi_inet_api_accept);
    lua_setfield(L, -2, "accept");
    lua_pushcfunction(L, nspr_luapi_inet_api_connect);
    lua_setfield(L, -2, "connect");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.inet table */
}
// XXX luapi_inet_api --> END

// XXX luapi_file_api --> BEGIN
static int nspr_luapi_file_api_read(lua_State *L) {
    size_t rlen = LUAL_BUFFERSIZE;
    int fd = lua_tonumber(L, 1);
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    for (;;) {
        char *p = luaL_prepbuffsize(&b, rlen);
        ssize_t nr = nspr_file_read(fd, p, rlen);
        if (nr == -1) {
            if (errno == EAGAIN) continue;
        }
        luaL_addsize(&b, nr);
        if ((size_t)nr < rlen) break;  /* eof? */
        else if (rlen <= (MAX_SIZE_T / 4))  /* avoid buffers too large */
            rlen *= 2;  /* double buffer size at each iteration */
    }

    luaL_pushresult(&b);  /* close buffer */
    return 1;
}

static int nspr_luapi_file_api_write(lua_State *L) {
    int fd = lua_tonumber(L, 1);
    const char *data = lua_tostring(L, 2);
    size_t len = lua_tonumber(L, 3);
    nspr_file_write(fd, data, len);
    return 1;
}

static int nspr_luapi_file_api_close(lua_State *L) {
    int fd = lua_tonumber(L, 1);
    nspr_file_close(fd);
    return 1;
}

static int nspr_luapi_file_api_popen(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_touserdata(L, 1);
    const char *cmd = lua_tostring(L, 2);
    FILE *fp = nspr_file_popen(cmd, "r");
    int fd = fileno(fp);
    node->fp = fp;
    lua_pushnumber(L, fd);
    return 1;
}

static int nspr_luapi_file_api_pclose(lua_State *L) {
    nspr_event_node_fd_t *node = (nspr_event_node_fd_t *)lua_topointer(L, 1);
    nspr_file_pclose(node->fp);
    return 1;
}

static void nspr_luapi_file_api(lua_State *L) {
    lua_pushliteral(L, "file");
    lua_newtable(L);    /*  .file table aka {} */

    lua_pushcfunction(L, nspr_luapi_file_api_read);
    lua_setfield(L, -2, "read");
    lua_pushcfunction(L, nspr_luapi_file_api_write);
    lua_setfield(L, -2, "write");
    lua_pushcfunction(L, nspr_luapi_file_api_close);
    lua_setfield(L, -2, "close");
    lua_pushcfunction(L, nspr_luapi_file_api_popen);
    lua_setfield(L, -2, "popen");
    lua_pushcfunction(L, nspr_luapi_file_api_pclose);
    lua_setfield(L, -2, "pclose");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.file table */
}
// XXX luapi_file_api --> END

// XXX luapi_timer_api --> START
static int nspr_luapi_timer_api_new(lua_State *L) {
    lua_newuserdata(L, sizeof(nspr_event_timer_t));
    return 1;
}

static int nspr_luapi_timer_api_set(lua_State *L) {
    nspr_event_timer_t *t;
    t = (nspr_event_timer_t *)lua_touserdata(L, 1);
    unsigned long timer = (unsigned long) lua_tonumber(L, 2);
    t->timer = timer;
    return 1;
}

static int nspr_luapi_timer_api_get(lua_State *L) {
    nspr_event_timer_t *t;
    t = (nspr_event_timer_t *)lua_touserdata(L, 1);
    lua_pushnumber(L, t->timer);
    return 1;
}

static int nspr_luapi_timer_api_add(lua_State *L) {
    nspr_event_timer_t *t;
    t = (nspr_event_timer_t *)lua_touserdata(L, 1);
    t->handler = nspr_luapi_event_api_timer;
    nspr_event_add_timer(t);
    return 1;
}

static int nspr_luapi_timer_api_del(lua_State *L) {
    nspr_event_timer_t *t;
    t = (nspr_event_timer_t *)lua_topointer(L, 1);
    nspr_event_del_timer(t);
    return 1;
}

static void nspr_luapi_timer_api(lua_State *L) {
    lua_pushliteral(L, "timer");
    lua_newtable(L);    /*  .timer table aka {} */

    lua_pushcfunction(L, nspr_luapi_timer_api_new);
    lua_setfield(L, -2, "new");
    lua_pushcfunction(L, nspr_luapi_timer_api_set);
    lua_setfield(L, -2, "set");
    lua_pushcfunction(L, nspr_luapi_timer_api_get);
    lua_setfield(L, -2, "get");
    lua_pushcfunction(L, nspr_luapi_timer_api_add);
    lua_setfield(L, -2, "add");
    lua_pushcfunction(L, nspr_luapi_timer_api_del);
    lua_setfield(L, -2, "del");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.timer table */
}
// XXX luapi_timer_api --> END

// XXX luapi_signal_api --> START
static int nspr_luapi_signal_api_new(lua_State *L) {
    lua_newuserdata(L, sizeof(nspr_event_signal_t));
    return 1;
}

static int nspr_luapi_signal_api_set(lua_State *L) {
    nspr_event_signal_t *s;
    s = (nspr_event_signal_t *)lua_touserdata(L, 1);
    int signum = lua_tonumber(L, 2);
    s->signum = signum;
    return 1;
}

static int nspr_luapi_signal_api_get(lua_State *L) {
    nspr_event_signal_t *s;
    s = (nspr_event_signal_t *)lua_touserdata(L, 1);
    lua_pushnumber(L, s->signum);
    return 1;
}

static int nspr_luapi_signal_api_add(lua_State *L) {
    nspr_event_signal_t *s;
    s = (nspr_event_signal_t *)lua_touserdata(L, 1);
    s->handler = nspr_luapi_event_api_signal;
    nspr_event_add_signal(s);
    return 1;
}

static int nspr_luapi_signal_api_del(lua_State *L) {
    nspr_event_signal_t *s;
    s = (nspr_event_signal_t *)lua_topointer(L, 1);
    nspr_event_del_signal(s);
    return 1;
}

static void nspr_luapi_signal_api(lua_State *L) {
    lua_pushliteral(L, "signal");
    lua_newtable(L);    /*  .signal table aka {} */

    lua_pushcfunction(L, nspr_luapi_signal_api_new);
    lua_setfield(L, -2, "new");
    lua_pushcfunction(L, nspr_luapi_signal_api_set);
    lua_setfield(L, -2, "set");
    lua_pushcfunction(L, nspr_luapi_signal_api_get);
    lua_setfield(L, -2, "get");
    lua_pushcfunction(L, nspr_luapi_signal_api_add);
    lua_setfield(L, -2, "add");
    lua_pushcfunction(L, nspr_luapi_signal_api_del);
    lua_setfield(L, -2, "del");

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */
    lua_setmetatable(L, -2);    /*  tie the metatable to param table */
    lua_rawset(L, -3);    /*  set nspr.signal table */
}
// XXX luapi_signal_api --> END
