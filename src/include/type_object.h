#ifndef TYPE_OBJECT_H
#define TYPE_OBJECT_H
#include <object.h>
#include <vm.h>
typedef long (* hash_func)(ray_object *);
typedef int (*equals_func)(ray_object *, ray_object *);
typedef ray_object* (*add_func)(ray_object *, ray_object *);
typedef ray_object* (*sub_func)(ray_object *, ray_object *);
typedef ray_object* (*mul_func)(ray_object *, ray_object *);
typedef ray_object* (*div_func)(ray_object *, ray_object *);
typedef string_object* (*str_func)(ray_object *);
typedef logic_block* (*invoke_func)(ray_object *, list_object*);
typedef void (*destruct_func)(ray_object*);


struct _type_object {
    OBJ_HEADER;
    char* name;
    hash_func __hash;
    equals_func __equals;
    add_func __add;
    sub_func __sub;
    mul_func __mul;
    div_func __div;
    str_func __str;
    invoke_func __invoke;
    destruct_func __destruct;
};


extern type_object base_type_object;

#endif // TYPE_OBJECT_H
