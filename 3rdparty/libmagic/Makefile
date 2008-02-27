CFLAGS=-Wall -Wshadow -Wcast-qual -pedantic -ansi -O3 -g

all: libmagic minifile 

libmagic: libmagic.a

libmagic.a: magic.o
	ar cru libmagic.a magic.o
	ranlib libmagic.a

magic.o: magic.c sysdep.h magic.h

minifile: libmagic.a minifile.o

minifile.o: minifile.c
