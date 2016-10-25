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

#define PUSH 128
#define POP 129
#define PRINT 130
#define LOAD_NAME 131

#define HAS_OPARG(op) (op & 128)

#define DEFAULT_CODE_SIZE 4
typedef struct _instr {
    unsigned char opcode;
    ray_object* oparg;
} instr;

typedef struct {
    instr *code;
    int code_max_len;
    int code_len;
    map_object* globals;
    map_object* locals;
} code_block;




code_block * init_code_block();

void add_op(code_block * block, unsigned char op);
void add_op_arg(code_block *block, unsigned char op, ray_object* val);


#endif
