all: hobbyos.iso

compile:
	make -f Makefile.compile compile

hobbyos.iso: compile
	make -f Makefile.build

clean:
	make -f Makefile.build clean

.PHONY: all compile clean
