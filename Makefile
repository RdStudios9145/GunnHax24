windows: game.c
	gcc -o game.exe game.c -L./raylib/ -lraylib_windows -lm
linux: game.c
	gcc -o game game.c -L./raylib/ -lraylib_linux -lm
