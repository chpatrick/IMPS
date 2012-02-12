#include "hashtable.h"

// instruction types
#define OPTYPE 0x0F

#define MISC    0x00
#define RTYPE   0x01
#define ITYPE   0x02
#define JTYPE   0x03
#define R1TYPE  0x04
#ifdef FANCY
#define R12TYPE 0x05
#define OPS 35
#else
#define OPS 18
#endif

// label modifiers
#define OPMOD  0xF0
#define BRANCH 0x10
#define IMPORT 0x20

#define HASHOP(NAME,OPCODE,TYPE) temp = (opinfo *) malloc(sizeof(opinfo));\
                                 temp->name = NAME;\
                                 temp->opcode = OPCODE;\
                                 temp->type = TYPE;\
                                 add(opinfotable, NAME, temp); 
typedef struct opinfo {
  char *name;
  char opcode;
  int type;
} opinfo;


hashtable opinfotable;

void initoptable();
