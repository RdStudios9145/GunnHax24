windows: game.c
	gcc -o game.exe game.c -L./raylib/ -lraylib -lm
linux: game.c
	gcc -o game game.c -L./raylib/ -lraylib -lm
