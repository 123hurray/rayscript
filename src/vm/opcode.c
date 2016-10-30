#include <stdlib.h>
#include <allocator.h>
#include <opcode.h>
#include <vm.h>
#include <map_object.h>

static double scale_factor = 1.5;

logic_block *new_logic_block() {
    logic_block * lb = R_MALLOC(logic_block);
    lb->head_block = new_code_block();
    lb->eval_block = lb->cb = NULL;
    lb->globals = new_map_object(1);
    lb->locals = new_map_object(1);
    lb->consts = new_list_object(1);
    return lb;
}

void continue_compiler(compiler* c) {
    if(c->lb->cb) {
        c->lb->eval_block = c->lb->cb = new_code_block();
    } else {
        c->lb->eval_block = c->lb->cb = c->lb->head_block;
    }

}

compiler* new_compiler() {
    compiler * c = R_MALLOC(compiler);
    c->lb = new_logic_block();
    return c;
}

code_block *new_code_block() {
    code_block * c = R_MALLOC(code_block);
    c->code_max_len = DEFAULT_CODE_SIZE;
    c->code = R_MALLOC_N(instr, DEFAULT_CODE_SIZE);
    memset(c->code, 0, DEFAULT_CODE_SIZE);
    c->code_len = 0;
    c->next = NULL;
    return c;
}




void ensure_size(code_block *block) {
    if(block->code_len == block->code_max_len) {
        block->code_max_len = (int)(block->code_max_len * scale_factor);
        block->code = (instr *)realloc(block->code, block->code_max_len * sizeof(instr));
        memset(block->code + block->code_len, 0, block->code_max_len - block->code_len);
    }
}

void use_code_block_next(compiler *c, code_block *nb) {
    c->lb->cb->next = nb;
    c->lb->cb = nb;
    c->lb->cb->next = NULL;
}

void add_op(compiler *c, unsigned char op) {
    code_block *block = c->lb->cb;
    ensure_size(block);
    instr *i = &block->code[block->code_len];
    i->opcode = op;
    i->oparg = -1;
    ++block->code_len;
}

void add_op_arg(compiler *c, unsigned char op, int val) {
    code_block *block = c->lb->cb;
    ensure_size(block);
    instr * i = &block->code[block->code_len];
    i->opcode = op;
    i->oparg = val;
    ++block->code_len;
}
void add_op_jmp(compiler *c, unsigned char op, code_block* b) {
    code_block *block = c->lb->cb;
    ensure_size(block);
    instr * i = &block->code[block->code_len];
    i->opcode = op;
    i->oparg = -1;
    i->jmp_block = b;
    ++block->code_len;
}
