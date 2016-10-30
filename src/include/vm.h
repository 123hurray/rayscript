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


#define HANDLE_ARG(op) \
HANDLE_##op: \
case op:\
DEBUG_OP_ARG(#op);



#ifdef VM_DEBUG

#define DEBUG_OP(v) {\
    R_DEBUG("\n%p:%-6d%-28s", b, i, v);\
}



#define DEBUG_OP_ARG(v) {\
    R_DEBUG("\n%p:%-6d%-12s%3d %10s  ", b, i, v, arg, STRING_OBJ_AS_STRING(OBJ_STR(list_get(AS_OBJ((c)->lb->consts), (arg)))));\
}


#else

#define DEBUG_OP(v)
#define DEBUG_OP_ARG(v)

#endif


#define STACK_POP() ( \
    --stack_pos,\
    (stack_pos < 0)?  \
       (NULL) \
        :stack[stack_pos]);{if(stack_pos<0) QUIT_VM("Stack empty");}

#define STACK_PUSH(v) do { \
    if(stack_pos >= STACK_SIZE) { \
        R_FATAL("Stack overflow!"); \
        exit(-1); \
    } \
    stack[stack_pos] = v; \
    ++stack_pos; \
} while(0)
    
#define STACK_GET(v) (\
    (stack_pos < 1)? \
        (ray_object*)R_FATAL("Stack empty") \
        :stack[stack_pos-1])


// Structs

typedef struct _code_block code_block;
typedef struct _instr {
    unsigned char opcode;
    int oparg;
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
    list_object *consts;
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
void add_op_arg(compiler *, unsigned char, int);

void add_op_jmp(compiler *c, unsigned char op, code_block* b);



#endif
