#ifndef VM_H
#define VM_H

#include <opcode.h>
#include <vm.h>



// constants

#define STACK_SIZE 100
#define DEFAULT_CODE_SIZE 4



// Defines


#define HANDLE(op) \
HANDLE_##op: \
case op:\
DEBUG_OP(#op);



#ifdef VM_DEBUG

#define DEBUG_OP(v) {\
    R_DEBUG(v"\n");\
}


#else

#define DEBUG_OP(v)

#endif


#define STACK_POP() ( \
    --stack_pos,\
    (stack_pos < 0)? ( \
        R_FATAL("Stack empty!") \
        ):0, stack[stack_pos])

#define STACK_PUSH(v) do { \
    if(stack_pos >= STACK_SIZE) { \
        R_FATAL("Stack overflow!"); \
        exit(-1); \
    } \
    stack[stack_pos] = v; \
    ++stack_pos; \
} while(0);
    
#define STACK_GET(v) (\
    (stack_pos < 1)? (\
        R_FATAL("Stack empty") \
        ):0, stack[stack_pos-1])


// Structs

typedef struct _code_block code_block;
typedef struct _instr {
    unsigned char opcode;
    ray_object* oparg;
    code_block* jmp_block;
} instr;

struct _code_block {
    instr *code;
    int code_max_len;
    int code_len;
    struct _code_block *next;
};

typedef struct {
    code_block *head_block;
    code_block *eval_block;
    code_block *cb;
    map_object *globals;
    map_object *locals;
} logic_block;

typedef struct {
    logic_block *lb;
} compiler;

// Functions

void eval(compiler *);
void continue_compiler(compiler *);
logic_block *new_logic_block();
compiler* new_compiler();
code_block * new_code_block();

void use_code_block_next(compiler *c, code_block *nb);
void add_op(compiler *, unsigned char);
void add_op_arg(compiler *, unsigned char, ray_object*);

void add_op_jmp(compiler *c, unsigned char op, code_block* b);



#endif
