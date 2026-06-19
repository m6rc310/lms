all:		z80 spectrum

z80:		z80.o string.o system.o sysasm.o
	ld -ez80_main z80.o string.o system.o sysasm.o -o z80

z80.o:		z80.asm
	nasm -f elf64 z80.asm

z80.asm:		z80.lms
	../lmsc z80.lms z80.asm

spectrum:		spectrum.o z80.o string.o system.o sysasm.o
	gcc -no-pie -nostdlib -espectrum_main spectrum.o z80.o string.o system.o sysasm.o -lX11 -o spectrum

spectrum.o:		spectrum.asm
	nasm -f elf64 spectrum.asm

spectrum.asm:		spectrum.lms
	../lmsc spectrum.lms spectrum.asm

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
	rm *.o *.asm z80 spectrum
