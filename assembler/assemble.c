#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#ifdef FANCY
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <unistd.h>
#endif

#include "opinfo.h"
#include "hashtable.h"

#define MAX_LINE 1024
#define DELIMITER ", \r\t\n"
#define WARN(MSG, ...) if (verbose) {fprintf(stderr, MSG, ## __VA_ARGS__);}

typedef struct labelRef {
  char *name;
  uint16_t address;
  int modifiers;

  struct labelRef *next;
} labelRef;

#ifdef FANCY
typedef struct exportLabelList {
  char *name;
  uint16_t address;
  struct exportLabelList* next;
}
exportLabelList;

int numExportLabels = 0;
exportLabelList* exportHead;
#endif

typedef union word {
  uint32_t u;
  int32_t i;
  float f;
} word;

uint8_t rom[0x10000];
int numImportLabels = 0;
hashtable labels = NULL;
labelRef *labelRefRoot = NULL;
uint16_t pc;
int lineno;

int verbose = 0;

#ifdef FANCY
void convertTwoDigits(char *number) {
  if (number[1] == '\0') {
    number[2] = '\0';
    number[1] = number[0];
    number[0] = '0';
  }
}

void printWithLength(char *str, FILE* fp) {
  char numChars[3];
  sprintf(numChars, "%d", strlen(str));
  convertTwoDigits(numChars);
  fputs(numChars, fp);
  fputs(str, fp);
}

void printIntWithLength(int toPrint, FILE* fp) {
  char number[10];
  sprintf(number, "%d", toPrint);
  fprintf(fp, "%d", strlen(number));
  fputs(number, fp);
}
#endif

word getWord(uint16_t address) {
  word value;
  memcpy(&value, rom + address, 4);

#ifdef __BIG_ENDIAN__
  value.u = __builtin_bswap32(value.u);
#endif

  return value;
}

void setWord(uint16_t address, word value) {
#ifdef  __BIG_ENDIAN__
  value.u = __builtin_bswap32(value.u);
#endif

  memcpy(rom + address, &value, 4);
}

int parseLabel(char *token) {
  int len = strlen(token);
  if (token[len - 1] == ':') {
    // take the last character off
    char *newname = malloc(len);
    memcpy(newname, token, len - 1);
    newname[len - 1] = 0;

    uint16_t *value = malloc(sizeof(uint16_t));
    *value = pc;

#ifdef FANCY
    if (newname[0] == '~') {
      newname++;
      numExportLabels++;
      exportLabelList* newLabel = malloc(sizeof(exportLabelList));
      newLabel->next = exportHead;
      newLabel->name = newname;
      newLabel->address = pc;
      exportHead = newLabel;
    }
#endif
    add(labels, newname, (void *) value);

    WARN("Parsed label %s\n", newname);
    return 1;
  }
  else
    return 0;
}

void error(char *msg) {
  fprintf(stderr, "%s on line %d\n", msg, lineno);
  exit(1);
}

void syntax() {
  error("Syntax error");
}

char *getToken() {
  char *token = strtok(NULL, DELIMITER);
  if (token == NULL) syntax();
  return token;
}

unsigned int parseReg(unsigned int n) {
  unsigned int rnum;
  char *token = getToken();

  if (sscanf(token, "$%u", &rnum)) {
    if (rnum > 31) error("Register index out of bounds");

    WARN("Parsed register %u\n", rnum);
    return rnum << (26 - 5 * n);
  }
 
  syntax();
  return 0;
}

uint16_t parseVal(int lmod) {
  char *token = getToken();

  unsigned int val = 0;
  if (sscanf(token, "0x%x", &val)) {
    if (val > 0xffff) error("Hex literal out of bounds");
    
    WARN("Parsed hex %x\n", val);
    return val & 0xffff;
  }

  if (sscanf(token, "%d", &val)) {
    if (val > 0xffff && (val >> 16) != 0xffff) error("Decimal literal out of bounds");

    WARN("Parsed decimal %d\n", val);
    return val & 0xffff;
  }

  // assume it's a label
  size_t tlen = strlen(token);

  labelRef *lref = malloc(sizeof(lref));
  lref->name = malloc(tlen + 1);
  strcpy(lref->name, token);
  lref->address = pc;
  #ifdef FANCY
  if (strrchr(token, '@') != NULL) {
    lmod |= IMPORT;
    numImportLabels++;
  }
  #endif
  lref->modifiers = lmod;

  lref->next = labelRefRoot;
  labelRefRoot = lref;
 
  WARN("Parsed labelref %s\n", lref->name);
  return 0; // set it to 0 until we resolve the label
}

void parseLine(char* line) {
  char* token = strtok(line, DELIMITER);
  if (token == NULL || token[0] == ';') return; // empty line or comment

#ifdef FANCY
  // cpp linemarker
  if (token[0] == '#') {
    token = getToken();

    if (sscanf(token, "%d", &lineno)) {
      WARN("Set line number to %d\n", lineno);
      lineno--;
    }

    return;
  }
#endif

  while (parseLabel(token)) {
    token = strtok(NULL, DELIMITER);
    if (!token)
      return;
  }
 
  if (strcmp(token, ".fill") == 0) {
    token = getToken();
    word val;
    val.u = 0;
    if (sscanf(token, "0x%x", &val.u)) {
      WARN("Parsed hex %x\n", val.u);
    }
    else if (sscanf(token, "f%f", &val.f)) {
      WARN("Parsed float %f\n", val.f);
    }
    else if (sscanf(token, "%d", &val.i)) {
      WARN("Parsed decimal %d\n", val.i);
    }
    else
      syntax();

    setWord(pc, val);

    pc += 4;
    return;
  }

  if (strcmp(token, ".skip") == 0) {
    token = getToken();
    int count = 0;
    if (!sscanf(token, "%u", &count)) syntax();

    pc += 4 * count;
    return;
  }

  opinfo *opinfo = search(opinfotable, token);

  if (opinfo == NULL) error("Invalid opcode");
  WARN("Parsed op %s\n", opinfo->name);

  int lmod = opinfo->type & OPMOD;
  uint32_t insr = opinfo->opcode << 26;
  switch (opinfo->type & OPTYPE) {
    case MISC: break;
    case RTYPE: insr |= parseReg(1) | parseReg(2) | parseReg(3); break;
    case ITYPE: insr |= parseReg(1) | parseReg(2) | parseVal(lmod); break;
    case JTYPE: insr |= parseVal(lmod); break;
    case R1TYPE: insr |= parseReg(1); break;
#ifdef FANCY
    case R12TYPE: insr |= parseReg(1) | parseReg(2); break;
#endif
  }

  word insrWord;
  insrWord.u = insr;
  setWord(pc, insrWord);
  pc += 4;
  return;
}

int main(int argc, char** argv) {

  labels = newTable();
  initoptable();
  
  char *source;
  char *dest;
  FILE *fp;

#ifndef FANCY
  if (argc == 3) {
    source = argv[1];
    dest = argv[2];
  }
  else {
    fprintf(stderr, "Usage: assemble source dest\n");
    exit(64);
  }
#else
  int c;
  int qflag = 0;
  opterr = 0;

  while ((c = getopt(argc, argv, "qv")) != -1) {
    switch (c) {
      case 'v' :
        verbose = 1;
        break;
      case 'q' :
        qflag = 1;
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

  if (optind + 2 != argc){
    fprintf(stderr, "Usage: assemble [options] source dest\naccepted options:\n-v: verbose output\n-q: suppress standard messages\n");
    exit(64);
  }
  else{
    source = argv[optind];
    dest = argv[optind+1];
  }
#endif

#ifdef FANCY
  int rv;

  int cppout[2];
  pipe(cppout);

  int pid = fork();

  if (pid == 0) {
#ifdef FANCY
    if (!qflag)
#endif
    fprintf(stderr, "Preprocessing...\n");
    dup2(cppout[1], 1);
    close(cppout[0]);
    execlp("cpp", "cpp", "-traditional-cpp", source, NULL);
  }

  usleep(10000); // wait a bit for it to start
  waitpid(-1, &rv, WNOHANG);
  if (WIFSTOPPED(rv)) {
    fprintf(stderr, "cpp exited prematurely\n");
    exit(1);
  }

  // parent process
  fp = fdopen(cppout[0], "r");
  close(cppout[1]);
#else
  fp = fopen(source, "r");
  if (!fp) { perror("Unable to open source file"); exit(1); }
#endif
    
#ifdef FANCY
  if (!qflag)
#endif
  fprintf(stderr, "Assembling...\n");

  pc = 0;
  lineno = 1;
  do {
    char *line = NULL;
    size_t size = 0;
    if (getline(&line, &size, fp)) {
      WARN("%s", line);

      parseLine(line);

      lineno++;
    }
    else if (ferror(fp))
    {
      perror("Error while reading from file\n");
      exit(1);
    }
  } while (!feof(fp));

  fclose(fp);

#ifdef FANCY
  wait(&rv); // wait for cpp to finish
  close(cppout[0]);

  if (rv) {
    fprintf(stderr, "cpp didn't execute successfully\n");
    exit(1);
  }
#endif

#ifdef FANCY
  if (!qflag)
#endif
  fprintf(stderr, "Resolving labels...\n");
  fp = fopen(dest, "w");
  if (!fp) { perror("Unable to open dest file"); exit(1); }
  
#ifdef FANCY
  if (numImportLabels != 0 || numExportLabels != 0) {
    fputs("HEA~", fp);
    char numImports[3];
    sprintf(numImports, "%d", numImportLabels);
    convertTwoDigits(numImports);
    fputs(numImports, fp);
  }
#endif

  labelRef *lblRef;
  for (lblRef = labelRefRoot; lblRef != NULL; lblRef = lblRef->next) {
#ifdef FANCY
    if (lblRef->modifiers & IMPORT) {
      WARN("Not resolving external ref %s\n", lblRef->name);
      char *label = strtok(lblRef->name, "@");
      char *file = strtok(NULL, "@");
      sscanf(lblRef->name, "%s@%s", label, file);
      printWithLength(label, fp);
      printWithLength(file, fp);
      printIntWithLength(lblRef->address, fp);
      continue;
    }
#endif

    WARN("Resolving label ref %s\n", lblRef->name);

    uint16_t *addrptr = (uint16_t *) search(labels, lblRef->name);
    if (addrptr == NULL) {
      fprintf(stderr, "Reference to undefined label %s\n", lblRef->name); exit(1);
    }

    uint16_t addr = *addrptr;
    if (lblRef->modifiers & BRANCH) 
      addr = (addr - lblRef->address) >> 2;
    WARN("Setting %s at %04x to %04x\n", lblRef->name, lblRef->address, addr);
    word insr = getWord(lblRef->address);
    insr.u |= addr;
    setWord(lblRef->address, insr);
  }

#ifdef FANCY
  if (numImportLabels != 0 || numExportLabels != 0) {
    char numExports[3];
    sprintf(numExports, "%d", numExportLabels);
    convertTwoDigits(numExports);
    fputs(numExports, fp);
  
    exportLabelList *expLabel;
    for (expLabel = exportHead; expLabel != NULL; expLabel = expLabel->next ) {
      printWithLength(expLabel->name, fp);
      printIntWithLength(expLabel->address, fp);
    }
  }
  
  if (!qflag)
#endif
  fprintf(stderr, "Saving...\n");
  fwrite(rom, 1, pc, fp);
  if (!fp) { perror("Unable to write dest file"); exit(1); }
  fclose(fp);
  // used  to have freeing up code here, but there's no point.
#ifdef FANCY
  if (!qflag)
#endif
  fprintf(stderr, "Finished. Goodbye!\n");
  return 0;
}
