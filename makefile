#DEBUG = -g -DDEBUG -nostdinc -nostdlib
DEBUG = -nostdinc -nostdlib

lmsc:		lmsc.o tools.o syscalltool.o
	gcc $(DEBUG) -o lmsc lmsc.o tools.o syscalltool.o

lmsc.o:		lmsc.c define.h struct.h
	gcc $(DEBUG) -c lmsc.c

tools.o:		tools.c struct.h
	gcc $(DEBUG) -c tools.c

syscalltool.o:		syscalltool.asm
	nasm -f elf64 syscalltool.asm

clean:
		rm *.o lmsc
