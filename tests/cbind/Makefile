CC      = cc
CFLAGS  = -Wall 
INC     = -I/usr/include/lua5.2
LIBS    = -lm -llua5.2

TARGETS = cbind

all: $(TARGETS)

cbind: cbind.c
	$(CC) -o $@ $< $(CFLAGS) $(INC) $(LIBS)

clean:
	rm -rf $(TARGETS)
