#include <stdint.h>
#include <string.h>

#pragma once

typedef union { 
    uint32_t u;
    int32_t  i;
    float    f;
} word;

int halted;
word r[32];
uint8_t memory[0x10000];
uint16_t pc;
#ifdef FANCY
uint16_t sp;
#endif

uint32_t getWord(uint16_t address);
void setWord(uint16_t address, uint32_t value);