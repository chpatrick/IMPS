#include <stdlib.h>
#include <stdio.h>
#include "linker.h"
#include <libgen.h>

char *workingdir;

char* prependDir(char *filename) {
    char *temp = malloc(strlen(workingdir)+strlen(filename)+2);
    temp = strcpy(temp, workingdir);
    temp = strcat(temp, "/");
    temp = strcat(temp, filename);
    return temp;
}

char* readString(FILE *fp) {
  char numChars[3] = {fgetc(fp), fgetc(fp), '\0'};
  uint8_t length = atoi(numChars);
  char *str = malloc(length+1);
  int i;
  for (i = 0; i < length; i++) {
    *(str+i) = fgetc(fp);
  }
  str[length] = '\0';
  return str;
}

void addFile(char *filename) {
  if (numFiles == 0) {
    char *filenamecopy = malloc(strlen(filename)+1);
    filenamecopy = strcpy(filenamecopy, filename);
    workingdir = dirname(filenamecopy);
  }
  int i;
  for (i = 0; i < numFiles; i++) {
    fileinfo *currFile = files+i;
    if (!strcmp(currFile->filename, filename)) {
      return;
    }
  }
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    printf("Invalid filename\n");
    exit(1);
  }
  char header[4];
  fread(header, 1, 4, fp);
  int linking = 0;
  if (!strncmp(header, "HEA~", 4)) {
    linking = 1;
    char c[2] = {'\0', '\0'};
    char nums[3] = {fgetc(fp), fgetc(fp), '\0'};
    uint8_t numImports = atoi(nums);
    import *imports = malloc(sizeof(import)*numImports);
    int i;
    for (i = 0; i < numImports; i++) {
      (imports+i)->labelname = readString(fp);
      (imports+i)->filename = readString(fp);
      (imports+i)->filename = prependDir((imports+i)->filename);
      c[0] = fgetc(fp);
      int addressSize = atoi(c);
      int z;
      char address[addressSize + 1];
      for (z = 0; z < addressSize; z++) {
        address[z] = fgetc(fp);
      }
      address[addressSize] = '\0';
      (imports+i)->address = atoi(address);
    }
    nums[0] = fgetc(fp);
    nums[1] = fgetc(fp);
    uint8_t numExports = atoi(nums);
    export *exports = malloc(sizeof(export)*numExports);
    for (i = 0; i < numExports; i++) {
      (exports+i)->labelname = readString(fp);
      c[0] = fgetc(fp);
      int addressSize = atoi(c);
      char address[addressSize + 1];
      int z;
      for (z = 0; z < addressSize; z++) {
        address[z] = fgetc(fp);
      }
      address[addressSize] = '\0';
      (exports+i)->address = atoi(address);
    }
    numFiles++;
    files = realloc(files, sizeof(fileinfo)*numFiles);
    (files+numFiles-1)->filename = filename;
    (files+numFiles-1)->imports = imports;
    (files+numFiles-1)->exports = exports;
    (files+numFiles-1)->numExports = numExports;
    (files+numFiles-1)->numImports = numImports;
  }
  else {
    fseek(fp, 0, SEEK_SET);
  }
  uint16_t currOffset = (uint16_t)fread(memory+offset, 1, 0x10000-offset, fp);
  fclose(fp);
  uint32_t pc;
  for (pc = offset; pc < 65536-offset; pc += 4) {
    uint32_t insr = getWord(pc);
    unsigned int opcode = insr >> 26;
    if (opcode == 2 || opcode == 4 || opcode == 6 || opcode == 7 || opcode == 8 || opcode == 15 || opcode == 17) {
      insr += offset;
      setWord(pc, insr);
    }
  }
  if (linking) {
    (files+numFiles-1)->offset = offset;
    offset += currOffset;
    fileinfo *currFile = files+numFiles-1;
    for (i = 0; i < (currFile->numImports); i++) {
      addFile((currFile->imports)[i].filename);
    }
  }
}

void startLink(char *filename) {
  numFiles = 0;
  offset = 0;
  addFile(filename);
  int i, j, k;
  for (i = 0; i < numFiles; i++) {
    fileinfo *currFile = files+i;
    for (j = 0; j < currFile->numImports; j++) {
      import *currImport = (currFile->imports)+j;
      for (k = 0; k < numFiles; k++) {
        if (!strcmp(files[k].filename, currImport->filename))
          break;
      }
      fileinfo *toImport = files+k;
      export *exports = toImport->exports;
      uint16_t offset = toImport->offset;
      for (k = 0; k < numFiles; k++) {
        if (!strcmp(exports[k].labelname, currImport->labelname))
          break;
      }
      uint16_t wordAddress = currFile->offset + currImport -> address;
      uint32_t word = getWord(wordAddress); 
      uint16_t newOperand = exports[k].address + toImport->offset;
      word &= 0xffff0000;
      word |= newOperand;
      setWord(wordAddress, word);
    }
  }
}
