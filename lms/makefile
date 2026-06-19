all:		lmsc string system complex param sysasm.o

lmsc:		lmsc.o string.o system.o sysasm.o
	ld -elmsc_main lmsc.o string.o system.o sysasm.o -o lmsc

lmsc.o:		lmsc.asm
	nasm -f elf64 lmsc.asm

lmsc.asm:		lmsc.lms
	../lmsc lmsc.lms lmsc.asm

complex:		complex.o string.o system.o sysasm.o
	ld -ecomplex_main complex.o string.o system.o sysasm.o -o complex

complex.o:		complex.asm
	nasm -f elf64 complex.asm

complex.asm:		complex.lms
	../lmsc complex.lms complex.asm

param:		param.o string.o system.o sysasm.o
	ld -eparam_main param.o string.o system.o sysasm.o -o param

param.o:		param.asm
	nasm -f elf64 param.asm

param.asm:		param.lms
	../lmsc param.lms param.asm

string:		string.o system.o sysasm.o
	ld -estring_main string.o system.o sysasm.o -o string

string.o:		string.asm
	nasm -f elf64 string.asm

string.asm:		string.lms
	../lmsc string.lms string.asm

system:			system.o sysasm.o
	ld -esystem_main system.o sysasm.o -o system
    
system.o:		system.asm
	nasm -f elf64 system.asm

system.asm:		system.lms
	../lmsc system.lms system.asm

sysasm.o:		sysasm.nasm
	nasm -f elf64 sysasm.nasm

clean:
	rm *.o *.asm lmsc string system complex param
