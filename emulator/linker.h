#include "hardware.h"

int numFiles;
int offset;

void addFile(char *filename);
void startLink(char *filename);

typedef struct import {
  char *filename;
  uint16_t address;
  char *labelname;
} import;

typedef struct export {
  char *labelname;
  uint16_t address;
} export;

typedef struct fileinfo {
  char *filename;
  int numExports;
  int numImports;
  export *exports;
  import *imports;
  uint16_t offset;
} fileinfo;

fileinfo *files;
