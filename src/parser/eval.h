#ifndef EVAL_H
#define EVAL_H
#include "opcode.h"
#define STACK_SIZE 100
void eval(code_block *);

#define HANDLE(op) \
HANDLE_##op: \
case op:


#define STACK_POP() ( \
    --stack_pos,\
    (stack_pos < 0)? ( \
        fprintf(stderr, "Stack empty!"), \
        exit(-1) \
        ):0, stack[stack_pos])

#define STACK_PUSH(v) do { \
    if(stack_pos >= STACK_SIZE) { \
        fprintf(stderr, "Stack overflow!"); \
        exit(-1); \
    } \
    stack[stack_pos] = v; \
    ++stack_pos; \
} while(0);
    
#endif
