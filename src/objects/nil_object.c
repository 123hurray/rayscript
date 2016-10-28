#include <nil_object.h>

type_object nil_type_object = {
    &base_type_object,
    0,
    "nil",
    default_hash
};


nil_object _nil = {
    &nil_type_object,
    1
};

nil_object *nil = &_nil;

