#include <object.h>
#include <map_object.h>

long default_hash(ray_object *self) {
    return (long)((void *)self);
}

bool_object bool_true, bool_false, *p_bool_true, *p_bool_false;
bool_object* default_equals(ray_object *self, ray_object* other) {
    return (self == other)?p_bool_true:p_bool_false;
}



/** number **/

long number_object_hash(ray_object* self) {
    return (long)NUMBER_OBJ_AS_NUMBER(self);
}
type_object number_type_object = {
    &base_type_object,
    7,
    "number",
    number_object_hash
};
number_object *new_number_object(double val) {
    number_object * n = (number_object *)malloc(sizeof(number_object));
    n->type = &number_type_object;
    n->size = 4;
    n->val = val;
    return n;
}


