#include <nil_object.h>

type_object nil_type_object = {
    &base_type_object,
    "nil",
    default_hash
};


nil_object _nil = {
    &nil_type_object,
};

nil_object *nil = &_nil;

