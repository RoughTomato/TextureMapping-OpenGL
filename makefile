CC=gcc
CFILE=texturemapping.c
FLAGS=-lglut -lGL -lGLU `sdl-config --cflags --libs` -lSDL_image
OUT=texturemapping

all:
	$(CC) $(CFILE) $(FLAGS) -o $(OUT)
