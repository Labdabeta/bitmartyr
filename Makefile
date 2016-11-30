.PHONY: clean windows

SOURCES=src/main.c src/bitmartyr.c src/bitmartyr_gui.c src/sleep.c

# Put the SDL2 lib install directory here
WIN_SDL2_LIB_DIR=
# Put the SDL2 include install directory here
WIN_SDL2_INCLUDE_DIR=

bitmartyr : $(SOURCES)
	$(CC) $^ -g -o $@ `sdl2-config --cflags` `sdl2-config --libs`

windows : bitmartyr.exe

bitmartyr.exe : $(SOURCES)
	$(CC) $^ -o $@ -I$(WIN_SDL2_INCLUDE_DIR) -L$(WIN_SDL2_LIB_DIR) -D_REENTRANT -lmingw32 -lSDL2main -lSDL2
