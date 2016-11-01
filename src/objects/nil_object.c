#include <nil_object.h>


string_object* nil_str_v;

string_object* nil_str(ray_object* ignore) {
    INC_REF(nil_str_v);
    return nil_str_v;
}

type_object nil_type_object = {
    INIT_HEADER(&base_type_object),
    "nil",
    default_hash,
    default_equals,
    DEFAULT_BIN_OPS,
    nil_str,
    NULL, 
    default_destruct,
};


nil_object *nil;
static nil_object _nil = {
    INIT_HEADER(&nil_type_object),
};

void init_nil_object() {
    nil_str_v = new_string_object("nil");
    nil = &_nil;
};


