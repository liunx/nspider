CC = cc
CFLAGS =  -pipe  -O -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g 
LINK =	$(CC)

ALL_INCS = -I src/core \
	-I src/event \
	-I src/os \
	-I src/common \
	-I objs

CORE_DEPS = src/core/nspider.h \
	    src/os/nspr_linux.h \
	    src/common/nspr_common.h

CORE_INCS = -I src/core \
	    -I src/event \
	    -I src/os \
	    -I src/common \
	    -I objs

objs/nspider: objs/src/core/nspider.o \
    objs/src/core/nspr_log.o \
    objs/nspr_modules.o
	$(LINK) -o objs/nspider \
	    objs/src/core/nspider.o \
	    objs/src/core/nspr_log.o \
	    objs/nspr_modules.o

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

objs/nspr_modules.o: $(CORE_DEPS) \
    objs/nspr_modules.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
	    -o objs/nspr_modules.o \
	    objs/nspr_modules.c

clean:
	rm -rf objs/nspider \
	    objs/nspr_modules.o \
	    objs/src/core/nspider.o \
	    objs/src/core/nspr_log.o
