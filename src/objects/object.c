#include <object.h>
#include <map_object.h>

long default_hash(ray_object *self) {
    return (long)((void *)self);
}

bool_object bool_true, bool_false, *p_bool_true, *p_bool_false;
bool_object* default_equals(ray_object *self, ray_object* other) {
    return (self == other)?p_bool_true:p_bool_false;
}



