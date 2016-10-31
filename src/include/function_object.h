#ifndef FUNCTION_OBJECT_H
#define FUNCTION_OBJECT_H
#include <object.h>
#include <vm.h>

void init_function_object();
struct _function_object {
    OBJ_HEADER;
    int arg_len;
    list_object* arg_list;
    logic_block* code;
};
extern type_object function_type_object;

function_object* new_function_object(list_object* arg_list, logic_block*, int);
#endif // FUNCTION_OBJECT_H 
