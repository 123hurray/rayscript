#include <function_object.h>
#include <stdio.h>
#include <string.h>
#include <allocator.h>

#define AS_FUNCTION(f) ((function_object*)(f))


void init_function_object() {
}

string_object* function_str(ray_object* self) {
    char *name = R_MALLOC_N(char, 23);
    sprintf(name, "<function %p>", self);
    return new_string_object(name); 
}

logic_block* function_invoke(ray_object* self, list_object* args) {
    function_object* f = AS_FUNCTION(self);
    map_object* m = new_map_object(f->arg_len);
    for(int i = 0; i < f->arg_len; ++i) {
        ray_object* key = list_get(AS_OBJ(f->arg_list), i);
        ray_object* val = list_get(AS_OBJ(args), i);
        map_put(AS_OBJ(m), key, val);
    }
    logic_block *lb = new_logic_block();
    lb->head_block = lb->eval_block = lb->cb = f->code->eval_block;
    lb->globals = f->code->globals;
    lb->locals = m;
    lb->consts = f->code->consts;
    return lb;
}

type_object function_type_object = {
    &base_type_object,
    "function",
    default_hash,
    default_equals,
    NULL,
    NULL,
    NULL,
    NULL,
    function_str,
    function_invoke
};

function_object* new_function_object(list_object* arg_list, logic_block* code, int arg_len) {
    function_object* f = NEW_OBJ(function_object);
    f->type = &function_type_object;
    f->code = code;
    f->arg_list = arg_list;
    f->arg_len = arg_len;
    return f;
}

