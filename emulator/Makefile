CC = gcc
CFLAGS = -Wall -march=native -O2 -pipe
CPPFLAGS = -DFANCY
LDFLAGS = -lGL -lGLU -lSDL -lreadline -lm

all: emulate

.INTERMEDIATE: emulate.o linker.o gltools.o hardware.o

emulate: linker.o gltools.o hardware.o

.PHONY: clean

clean:
	$(RM) emulate
