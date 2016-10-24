#include<stdlib.h>
#include<memory.h>
#include"opcode.h"

static double scale_factor = 1.5;

code_block * init_code_block() {
    code_block * c = (code_block *)malloc(sizeof(code_block));
    c->code_max_len = DEFAULT_CODE_SIZE;
    c->code = (instr *)malloc(DEFAULT_CODE_SIZE * sizeof(instr));
    memset(c->code, 0, DEFAULT_CODE_SIZE);
    c->code_len = 0;
}




void ensure_size(code_block *block) {
    if(block->code_len == block->code_max_len) {
        block->code_max_len = (int)(block->code_max_len * scale_factor);
        block->code = (instr *)realloc(block->code, block->code_max_len * sizeof(instr));
        memset(block->code + block->code_len, 0, block->code_max_len - block->code_len);
    }
}


void add_op(code_block * block, unsigned char op) {
    ensure_size(block);
    instr *i = &block->code[block->code_len];
    i->opcode = op;
    i->oparg = NULL;
    ++block->code_len;
}

void add_op_arg(code_block *block, unsigned char op, ray_object* val) {
    ensure_size(block);
    instr * i = &block->code[block->code_len];
    i->opcode = op;
    i->oparg = val;
    ++block->code_len;
}
