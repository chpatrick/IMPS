#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef FANCY
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <unistd.h>
#endif

#include "hardware.h"
#include "cpuops.h"
#include "linker.h"

void dump() {
  printf("\nRegisters:\n");
  printf("PC : %10d (0x%08x)\n", pc, pc);
#ifdef FANCY
  if (sp != 0xfffb) printf("SP :            (0x%08x)\n",sp);
#endif
  int i;
  for (i = 0; i < 32; i++)
    printf("$%-2d: %10d (0x%08x)\n", i, r[i].i, r[i].i);
}

#ifdef FANCY
void memdump() {
  int xxdin[2];

  pipe(xxdin);

  int pid = fork();

  if (pid) {
    close(xxdin[0]);
    write(xxdin[1], &memory, 0x10000);

    close(xxdin[1]);

    int rv;
    wait(&rv);
  }
  else if (pid == 0) {
    close(xxdin[1]);
    dup2(xxdin[0], 0);

    execlp("xxd", "xxd", "-b", "-c4", "-a", "-", NULL);
  }
  else perror("Could not execute xxd");
}

#endif
void step() {
    uint32_t insr = getWord(pc);

    unsigned int opcode = insr >> 26;
#ifdef FANCY
    if (pc >= sp) {
        printf("FATAL ERROR: STACK OVERFLOW");
        dump();
        exit(1);
    }

#endif
    if (opcode >= OPS) {
      printf("FATAL ERROR: invalid operation %d\n", opcode);
      dump();
#ifdef FANCY
//    memdump();
#endif
      exit(1);
    }

    opptr insrOp = optable[opcode];

    pc += 4;
    insrOp(insr);
}

int main(int argc, char* argv[]) {
  char *filename;
#ifdef FANCY
  int debug = 0;
#endif

#ifndef FANCY
  if (argc == 2) {
    filename = argv[1];
  }
  else {
    printf("Usage: emulate binary");
    exit(64);
  }
#else
  int c;
  opterr = 0;

  while ((c = getopt(argc, argv, "d")) != -1) {
    switch (c) {
      case 'd' :
        debug = 1;
        break;
      case '?' :
        if (isprint (optopt))
          fprintf(stderr, "Unknown option '-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        exit(64);
      default :
        abort();
    }
  }
  if (optind + 1 != argc){
    fprintf(stderr,"Usage: emulate [options] binary\naccepted options:\n-d: debug mode\n");
    exit(64);
  }
  else{
    filename = argv[optind];
  }

#endif
  
  pc = 0;
  memset(r, 0, sizeof(word) * 32);
  memset(memory, 0, 0x10000);
  halted = 0;
#ifdef FANCY
  sp = 0xfffb; // highest available word

  startLink(filename);
#else

  FILE *fp = fopen(filename, "r");
  if (!fp) { perror("Unable to open binary"); exit(1); }

  fread(&memory, 1, 0x10000, fp);

  fclose(fp);
#endif

#ifdef FANCY
  if (debug) {
    printf("Welcome to the IMPS debugger\nCommands: step stepd run dump mem exit\n");
    while (1) {
      char *command = readline(">>= ");
      if (command) {
	add_history(command);

	if (strcmp(command, "step") == 0) {
	  if (!halted)
	    step();
	  else
	    printf("Halted.\n");
	}
	else if (strcmp(command, "run") == 0) {
          if (halted)
	    printf("Halted.\n");
	  else
	    do step();
	    while (!halted);
	}
	else if (strcmp(command, "dump") == 0)
	  dump();
	else if (strcmp(command, "stepd") == 0) {
	  if (!halted) {
	    step();
	    dump();
	  }
	  else
	    printf("Halted.\n");
	}
	else if (strcmp(command, "mem") == 0) {
          memdump();
	}
	else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
	  return 0;
	else
	  printf("Invalid command\n");
      }
      else return 0;
    }
  }
  else {
    while (!halted)
      step();

    dump();
   }
#else
  while (!halted)
    step();
  
  dump();
#endif

  return 0;
}
