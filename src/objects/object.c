#include <object.h>
#include <map_object.h>
#include <stdio.h>

void init_objects() {
    init_bool_object();
    init_nil_object();
    init_function_object();
}


long default_hash(ray_object *self) {
    return (long)((void *)self);
}

bool_object bool_true, bool_false, *p_bool_true, *p_bool_false;
int default_equals(ray_object *self, ray_object* other) {
    return (self == other)? CMP_NOT_RELATIONAL_EQ : CMP_NOT_RELATIONAL_NE;
}


string_object* default_str(ray_object* self) {
    char s[256];
    sprintf(s, "[%s]", OBJ_GET_TYPE_NAME(self));
    return new_string_object(s);
}



