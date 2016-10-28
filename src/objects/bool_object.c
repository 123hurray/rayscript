#include <bool_object.h>

type_object bool_type_object = {
    &base_type_object,
    "bool",
    default_hash,
    default_equals
};

bool_object bool_true = {
    &bool_type_object,
};

bool_object bool_false = {
    &bool_type_object,
};

bool_object *p_bool_true = &bool_true;
bool_object *p_bool_false = &bool_false;

