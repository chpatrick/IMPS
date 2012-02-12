#include "opinfo.h"

void initoptable() {
  opinfotable = newTable();
  opinfo *temp;

  HASHOP("halt",  0, MISC )
  HASHOP("add",   1, RTYPE )
  HASHOP("addi",  2, ITYPE )
  HASHOP("sub",   3, RTYPE )
  HASHOP("subi",  4, ITYPE )
  HASHOP("mul",   5, RTYPE )
  HASHOP("muli",  6, ITYPE )
  HASHOP("lw",    7, ITYPE )
  HASHOP("sw",    8, ITYPE )
  HASHOP("beq",   9, ITYPE | BRANCH )
  HASHOP("bne",  10, ITYPE | BRANCH )
  HASHOP("blt",  11, ITYPE | BRANCH )
  HASHOP("bgt",  12, ITYPE | BRANCH )
  HASHOP("ble",  13, ITYPE | BRANCH )
  HASHOP("bge",  14, ITYPE | BRANCH )
  HASHOP("jmp",  15, JTYPE )
  HASHOP("jr",   16, R1TYPE )
  HASHOP("jal",  17, JTYPE )
  #ifdef FANCY
  HASHOP("push", 18, R1TYPE )
  HASHOP("pop" , 19, R1TYPE )
  HASHOP("mvsp", 20, R1TYPE )
  HASHOP("call", 21, JTYPE )
  HASHOP("ret" , 22, MISC )
  HASHOP("fadd", 23, RTYPE )
  HASHOP("fsub", 24, RTYPE )
  HASHOP("fmul", 25, RTYPE )
  HASHOP("fdiv", 26, RTYPE )
  HASHOP("itof", 27, R12TYPE )
  HASHOP("ftoi", 28, R12TYPE )
  HASHOP("shl" , 29, ITYPE )
  HASHOP("shr" , 30, ITYPE )
  HASHOP("sb"  , 31, ITYPE )
  HASHOP("gpu" , 32, JTYPE )
  HASHOP("fsin", 33, R12TYPE )
  HASHOP("tics", 34, R1TYPE )
  #endif
  
}
