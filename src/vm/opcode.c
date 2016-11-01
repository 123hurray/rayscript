#include <config.h>
#include <stdlib.h>
#include <allocator.h>
#include <opcode.h>
#include <vm.h>
#include <object.h>
#include <memory.h>


extern type_object logic_block_type_object;

static double scale_factor = 1.5;

c_cache *lb_cache_head;
void init_lb_cache() {
    lb_cache_head = R_MALLOC(c_cache);
    lb_cache_head->next = NULL;
}

logic_block* borrow_lb_from_cache() {
    if(lb_cache_head->next != NULL) {
        c_cache* item = lb_cache_head->next;
        lb_cache_head->next = item->next;
        INIT_OBJ_HEADER((logic_block*)item, logic_block_type_object);
        return (logic_block*) item;
    } else {
        logic_block* lb = NEW_OBJ(logic_block);
        INIT_OBJ_HEADER(lb, logic_block_type_object);
        lb->stack = R_MALLOC_N(ray_object*, STACK_SIZE);
        return lb;
    }
}

void return_to_lb_cache(logic_block* o) {
    c_cache *cache = (c_cache*)o;
    c_cache *next = lb_cache_head->next;
    lb_cache_head->next = cache;
    cache->next = next;
}



void destruct_code_block(code_block* self) {
    R_FREE(self->code);
    R_FREE(self);
}


void destruct_logic_block(ray_object* o) {
    logic_block* self = (logic_block*)o;
    DEC_REF(self->locals);
    DEC_REF(self->globals);
    DEC_REF(self->consts);
    /*code_block* cb = self->head_block;
    while(cb) {
        code_block* tmp = cb;
        cb = cb->next;
        destruct_code_block(tmp);
    }*/
    return_to_lb_cache(self);
}

type_object logic_block_type_object = {
    INIT_HEADER(&base_type_object),
    "logic_block",
    default_hash,
    default_equals,
    DEFAULT_BIN_OPS,
    default_str,
    NULL,
    destruct_logic_block,
};



logic_block *new_logic_block_from_exists(logic_block* old, map_object* locals) {
    logic_block* lb = borrow_lb_from_cache();
    INC_REF(locals);
    lb->locals = locals;
    lb->head_block = lb->eval_block = lb->cb = old->eval_block;

    INC_REF(old->globals);
    lb->globals = old->globals;


    INC_REF(old->consts);
    lb->consts = old->consts;
    return lb;
}


logic_block *new_logic_block(map_object* globals, map_object* locals) {
    logic_block * lb = borrow_lb_from_cache(); 
    lb->head_block = new_code_block();
    lb->eval_block = lb->cb = NULL;
    lb->globals = globals;
    INC_REF(globals);
    lb->locals = locals;
    INC_REF(locals);
    lb->consts = new_list_object(1);
    memset(lb->stack, 0, sizeof(ray_object*) * STACK_SIZE);
    lb->next = lb->prev = NULL;
    lb->pc = 0;
    lb->stack_pos = 0;
    return lb;
}

void compiler_push_logic_block(compiler* c, logic_block* l) {
    INC_REF(l);
    c->lb->next = l;
    l->prev = c->lb;
    c->lb = l;
}

logic_block* compiler_pop_logic_block(compiler* c) {
    logic_block* l = c->lb;
    c->lb = l->prev;
    if(c->lb != NULL) {
        c->lb->next = NULL;
    }
    l->prev = l->next = NULL;
    return l;
}

void continue_compiler(compiler* c) {
    if(c->lb->cb) {
        c->lb->eval_block = c->lb->cb = new_code_block();
    } else {
        c->lb->eval_block = c->lb->cb = c->lb->head_block;
    }

}

void destruct_compiler(compiler* c) {
    logic_block* lb = c->head_lb;
    if(lb) {
        DEC_REF(lb);
    }
    R_FREE(c);
}


compiler* new_compiler() {
    // XXX Not a good place to init...
    init_lb_cache();


    compiler * c = R_MALLOC(compiler);
    map_object* globals_n_locals = new_map_object(1);
    c->head_lb = c->lb = new_logic_block(globals_n_locals, globals_n_locals);
    DEC_REF(globals_n_locals);
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
