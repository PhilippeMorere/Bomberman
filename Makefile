CC = gcc -g
SDL_INCLUDES = `sdl-config --cflags`
SDL_LIBS = `sdl-config --libs`

DEPFLAGS = $(SDL_INCLUDES)
CFLAGS = -Wall -O -std=c99 $(SDL_INCLUDES) 
LDFLAGS = $(SDL_LIBS) -lSDL_image

SRC  = $(wildcard src/*.c)
OBJ  = $(SRC:.c=.o) 
EXEC = Bomberman


.PHONY: all dep clean mrproper

all : dep $(EXEC)
dep: .depend
.depend: $(SRC)
	@touch .depend
	$(CC) -MM $(DEPFLAGS) $(SRC) > $@

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f *~ $(OBJ)

mrproper: clean
	-rm -f *~ $(EXEC) .depend


ifneq ($(wildcard .depend),)
include .depend
endif
