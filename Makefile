program: main.o lib.o 
	ld -o program main.o lib.o 
lib.o: lib.asm
	nasm -g -felf64 -o lib.o lib.asm
main.o: main.asm
	nasm -g -felf64 -o main.o main.asm
clean:
	rm main.o lib.o program

main.asm :  commands.inc
commands.inc : macro.inc util.inc
