#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hardware.h"
#ifdef FANCY
#include "gpuops.h"
#endif

// op macros
#define R(N) r[(insr >> (26 - N * 5)) & 0x1f]
#define C ((int16_t) (insr & 0xffff))
#define A (insr & 0xffff)
#define OP(NAME) void NAME(uint32_t insr)
#define ALU(NAME, FUNC) OP(NAME) { R(1).i = R(2).i FUNC R(3).i; } \
                        OP(NAME ## i) { R(1).i = R(2).i FUNC C; }
#define FALU(NAME, FUNC) OP(NAME) { R(1).f = R(2).f FUNC R(3).f; }
#define BRANCH(NAME, CMP) OP(NAME) { if (R(1).i CMP R(2).i) pc += C * 4 - 4; }

// vanilla
OP(halt) { halted = 1; }
ALU(add, +)
ALU(sub, -)
ALU(mul, *)
OP(lw) { R(1).u = getWord(R(2).u + C); }
OP(sw) { setWord(R(2).u + C, R(1).u); }
BRANCH(beq, ==)
BRANCH(bne, !=)
BRANCH(blt, <)
BRANCH(bgt, >)
BRANCH(ble, <=)
BRANCH(bge, >=)
OP(jmp) { pc = A; }
OP(jr) { pc = R(1).u; }
OP(jal) { r[31].u = pc; pc = A; } // pc is preincremented
#ifdef FANCY

OP(push) { setWord(sp, R(1).u); sp -= 4; }
OP(pop) { sp += 4; R(1).u = getWord(sp); }
OP(mvsp) { R(1).u = sp; }

OP(call) {
  sp += 2;
// push half word (pc)
  uint16_t value = pc;
#ifdef  __BIG_ENDIAN__
  value = __builtin_bswap16(value);
#endif
  memcpy(memory + sp, &value, 2);
  sp -= 4;
  pc = A;
};

OP(ret) {
// pop half word to pc;
  sp += 4;
  uint16_t value;
  memcpy(&value, memory + sp, 2);
#ifdef __BIG_ENDIAN__
  value = __builtin_bswap16(value);
#endif
  pc = value;
  sp -= 2;
}

FALU(fadd, +)
FALU(fsub, -)
FALU(fmul, *)
FALU(fdiv, /)
OP(itof) { R(1).f = R(2).i; }
OP(ftoi) { R(1).i = R(2).f; }
OP(shl) { R(1).u = R(2).u << C; }
OP(shr) { R(1).u = R(2).u >> C; }
OP(sb) { memory[R(2).u + C] = R(1).u & 0xff; }

OP(gpu) {
  gpuptr gpucall = gpucalls[A];
  gpucall();
}
OP(fsin) {R(1).f = sin(R(2).f);}
OP(tics) {R(1).i = SDL_GetTicks();}

#define OPS 35
#else
#define OPS 18
#endif

typedef void (*opptr) (uint32_t);

opptr optable[OPS] = {
   [0] = halt,
   [1] = add,
   [2] = addi,
   [3] = sub,
   [4] = subi,
   [5] = mul,
   [6] = muli,
   [7] = lw,
   [8] = sw,
   [9] = beq,
  [10] = bne,
  [11] = blt,
  [12] = bgt,
  [13] = ble,
  [14] = bge,
  [15] = jmp,
  [16] = jr,
  [17] = jal,
#ifdef FANCY
  [18] = push,
  [19] = pop,
  [20] = mvsp,
  [21] = call,
  [22] = ret,
  [23] = fadd,
  [24] = fsub,
  [25] = fmul,
  [26] = fdiv,
  [27] = itof,
  [28] = ftoi,
  [29] = shl,
  [30] = shr,
  [31] = sb,
  [32] = gpu,
  [33] = fsin,
  [34] = tics
#endif
};
