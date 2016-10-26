#ifndef EVAL_H
#define EVAL_H
#include <opcode.h>
#include <vm.h>
#define STACK_SIZE 100
void eval(compiler *);

#define HANDLE(op) \
HANDLE_##op: \
case op:\
DEBUG_OP(#op);




#define DEBUG_OP(v) {\
    printf(v"\n");\
}

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
    
#define STACK_GET(v) (\
    (stack_pos < 1)? (\
        fprintf(stderr, "Stack empty"), \
        exit(-1) \
        ):0, stack[stack_pos-1])
#endif
