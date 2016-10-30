#ifndef OPCODE_H
#define OPCODE_H
#include "object.h"
#include "map_object.h"
typedef unsigned char op_t;


#define STOP 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define PRINT 5
#define JUMP_FALSE 6
#define JUMP_TRUE 7
#define JUMP 8
#define POP 9
#define DUP 10
#define OP_EQ 11
#define OP_NE 12
#define OP_LT 13
#define OP_GT 14
#define OP_LE 15
#define OP_GE 16

#define PUSH 128
#define STORE_NAME 129
#define LOAD_NAME 131


#define HAS_OPARG(op) (op & 128)


#endif
