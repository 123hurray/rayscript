#ifndef STRING_OBJECT_H
#define STRING_OBJECT_H
#include <object.h>

extern type_object string_type_object;
struct _string_object {
    OBJ_HEADER_WITH_SIZE;
    char* val;
    uchar is_hash_cached;
    long hash_cache;
};

type_object string_type_object;


string_object *new_string_object(char* val);
#define STRING_EXACT(o) ((((ray_object *)o)->type == &string_type_object)?p_bool_true:p_bool_false)

#define STRING_OBJ_AS_STRING(o) (((string_object *)(o))->val)

#endif // STRING_OBJECT_H
