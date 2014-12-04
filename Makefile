default: build

clean:
	make -f objs/Makefile clean
build:
	$(MAKE) -f objs/Makefile
