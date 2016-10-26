#ifndef VM_H
#define VM_H

#define DEFAULT_CODE_SIZE 4

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
    code_block *cb;
    map_object *globals;
    map_object *locals;
} logic_block;

typedef struct {
    logic_block *lb;
} compiler;




logic_block *new_logic_block();
compiler* new_compiler();
code_block * new_code_block();

void use_code_block_next(compiler *c, code_block *nb);
void add_op(compiler *, unsigned char);
void add_op_arg(compiler *, unsigned char, ray_object*);

void add_op_jmp(compiler *c, unsigned char op, code_block* b);



#endif
