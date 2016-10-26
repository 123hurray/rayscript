#ifndef OPCODE_H
#define OPCODE_H
#include "object.h"
#include "map_object.h"
typedef unsigned char op_t;


#define STOP 0
#define D_ADD 1
#define D_SUB 2
#define D_MUL 3
#define D_DIV 4
#define STORE_NAME 5
#define JUMP_FALSE 6
#define JUMP 7
#define POP 8
#define DUP 9

#define PUSH 128
#define PRINT 130
#define LOAD_NAME 131


#define HAS_OPARG(op) (op & 128)


#endif
