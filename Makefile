game: game.o:
	ld -o main main.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -L. -lc -lraylib -lm

game.o: game.asm
	fasm game.asm
