CC = cc
CFLAGS =  -pipe -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g
LINK =	$(CC)

ALL_INCS = -I src/core \
	   -I src/event \
	   -I src/os \
	   -I src/common \
	   -I src/thirdparts/list/src \
	   -I src/thirdparts/http-parser \
	   -I src/thirdparts/hash \
	   -I src/thirdparts/json \
	   -I src/luapi \
	   -I objs

CORE_DEPS = src/core/nspider.h \
	    src/core/nspr_log.h \
	    src/core/nspr_mem.h \
	    src/core/nspr_inet.h \
	    src/core/nspr_file.h \
	    src/core/nspr_timer.h \
	    src/event/nspr_event.h \
	    src/os/nspr_linux.h \
	    src/common/nspr_common.h \
	    src/thirdparts/list/src/list.h \
	    src/thirdparts/http-parser/http_parser.h \
	    src/thirdparts/hash/hash.h \
	    src/thirdparts/json/json.h \
	    src/luapi/luapi.h \

CORE_INCS = -I src/core \
	    -I src/event \
	    -I src/os \
	    -I src/common \
	    -I src/thirdparts/list/src \
	    -I src/thirdparts/http-parser \
	    -I src/thirdparts/hash \
	    -I src/thirdparts/json \
	    -I src/luapi \
	    -I objs

objs/nspider: objs/src/core/nspider.o \
    objs/src/core/nspr_log.o \
    objs/src/core/nspr_mem.o \
    objs/src/core/nspr_inet.o \
    objs/src/core/nspr_file.o \
    objs/src/core/nspr_timer.o \
    objs/src/event/nspr_event.o \
    objs/src/event/modules/nspr_select.o \
    objs/src/thirdparts/list/list.o \
    objs/src/thirdparts/list/list_node.o \
    objs/src/thirdparts/list/list_iterator.o \
    objs/src/thirdparts/http-parser/http_parser.o \
    objs/src/thirdparts/hash/hash.o \
    objs/src/thirdparts/json/json.o \
    objs/src/luapi/luapi.o \
    objs/nspr_modules.o
	$(LINK) -o objs/nspider \
	    objs/src/core/nspider.o \
	    objs/src/core/nspr_log.o \
	    objs/src/core/nspr_mem.o \
	    objs/src/core/nspr_inet.o \
	    objs/src/core/nspr_file.o \
	    objs/src/core/nspr_timer.o \
	    objs/src/event/nspr_event.o \
	    objs/src/event/modules/nspr_select.o \
	    objs/src/thirdparts/list/list.o \
	    objs/src/thirdparts/list/list_node.o \
	    objs/src/thirdparts/list/list_iterator.o \
	    objs/src/thirdparts/http-parser/http_parser.o \
	    objs/src/thirdparts/hash/hash.o \
	    objs/src/thirdparts/json/json.o \
	    objs/src/luapi/luapi.o \
	    objs/nspr_modules.o -llua5.2

objs/src/core/nspider.o: $(CORE_DEPS) \
    src/core/nspider.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/core/nspider.o \
	    src/core/nspider.c

objs/src/core/nspr_log.o: $(CORE_DEPS) \
    src/core/nspr_log.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/core/nspr_log.o \
	    src/core/nspr_log.c

objs/src/core/nspr_mem.o: $(CORE_DEPS) \
    src/core/nspr_mem.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/core/nspr_mem.o \
	    src/core/nspr_mem.c

objs/src/core/nspr_inet.o: $(CORE_DEPS) \
    src/core/nspr_inet.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/core/nspr_inet.o \
	    src/core/nspr_inet.c

objs/src/core/nspr_file.o: $(CORE_DEPS) \
    src/core/nspr_file.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/core/nspr_file.o \
	    src/core/nspr_file.c

objs/src/core/nspr_timer.o: $(CORE_DEPS) \
    src/core/nspr_timer.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/core/nspr_timer.o \
	    src/core/nspr_timer.c

objs/src/event/nspr_event.o: $(CORE_DEPS) \
    src/event/nspr_event.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/event/nspr_event.o \
	    src/event/nspr_event.c

objs/src/event/modules/nspr_select.o: $(CORE_DEPS) \
    src/event/modules/nspr_select.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/event/modules/nspr_select.o \
	    src/event/modules/nspr_select.c

objs/src/thirdparts/list/list.o: $(CORE_DEPS) \
    src/thirdparts/list/src/list.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/thirdparts/list/list.o \
	    src/thirdparts/list/src/list.c \

objs/src/thirdparts/list/list_node.o: $(CORE_DEPS) \
    src/thirdparts/list/src/list_node.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/thirdparts/list/list_node.o \
	    src/thirdparts/list/src/list_node.c \

objs/src/thirdparts/list/list_iterator.o: $(CORE_DEPS) \
    src/thirdparts/list/src/list_iterator.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/thirdparts/list/list_iterator.o \
	    src/thirdparts/list/src/list_iterator.c \

objs/src/thirdparts/http-parser/http_parser.o: $(CORE_DEPS) \
    src/thirdparts/http-parser/http_parser.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/thirdparts/http-parser/http_parser.o \
	    src/thirdparts/http-parser/http_parser.c

objs/src/thirdparts/json/json.o: $(CORE_DEPS) \
    src/thirdparts/json/json.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/thirdparts/json/json.o \
	    src/thirdparts/json/json.c

objs/src/thirdparts/hash/hash.o: $(CORE_DEPS) \
    src/thirdparts/hash/hash.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/src/thirdparts/hash/hash.o \
	    src/thirdparts/hash/hash.c

objs/src/luapi/luapi.o: $(CORE_DEPS) $(LIBLUA_INCS) \
    src/luapi/luapi.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -I$(LIBLUA_INCS) \
	    -o objs/src/luapi/luapi.o \
	    src/luapi/luapi.c

objs/nspr_modules.o: $(CORE_DEPS) \
    objs/nspr_modules.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/nspr_modules.o \
	    objs/nspr_modules.c

clean:
	rm -rf objs/nspider \
	    objs/nspr_modules.o \
	    objs/src/core/nspider.o \
	    objs/src/core/nspr_log.o \
	    objs/src/core/nspr_mem.o \
	    objs/src/core/nspr_inet.o \
	    objs/src/core/nspr_file.o \
	    objs/src/core/nspr_timer.o \
	    objs/src/event/nspr_event.o \
	    objs/src/thirdparts/list/list.o \
	    objs/src/thirdparts/list/list_node.o \
	    objs/src/thirdparts/list/list_iterator.o \
	    objs/src/thirdparts/http-parser/http_parser.o \
	    objs/src/thirdparts/hash/hash.o \
	    objs/src/thirdparts/json/json.o \
	    objs/src/luapi/luapio \
	    objs/src/event/modules/nspr_select.o
