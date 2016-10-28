#ifndef TYPE_OBJECT_H
#define TYPE_OBJECT_H
#include <object.h>

typedef long (* hash_func)(ray_object *);
typedef bool_object* (*equals_func)(ray_object *, ray_object *);



struct _type_object {
    OBJ_HEADER;
    char* name;
    hash_func __hash;
    equals_func __equals;
};


extern type_object base_type_object;

#endif // TYPE_OBJECT_H
