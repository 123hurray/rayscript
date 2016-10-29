#include <nil_object.h>


string_object* nil_str_v;

string_object* nil_str(ray_object* ignore) {
    return nil_str_v;
}

type_object nil_type_object = {
    &base_type_object,
    "nil",
    default_hash,
    default_equals,
    NULL,
    NULL,
    NULL,
    NULL,
    nil_str
};


nil_object *nil;
static nil_object _nil = {
    &nil_type_object,
};

void init_nil_object() {
    nil_str_v = new_string_object("nil");
    nil = &_nil;
};


