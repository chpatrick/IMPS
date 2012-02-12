CC = gcc
CFLAGS = -Wall -march=native -O2 -pipe
CPPFLAGS = -DFANCY

all: assemble

.INTERMEDIATE: assemble.o hashtable.o opinfo.o

assemble: hashtable.o opinfo.o

.PHONY: clean

clean:
	$(RM) assemble
