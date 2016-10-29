#include <bool_object.h>
string_object* true_str;
string_object* false_str; 

void init_bool_object() {
    true_str = new_string_object("true");
    false_str = new_string_object("false");
}

string_object* bool_str(ray_object* self) {
    if(OBJ_IS_TRUE(self)) {
        return true_str;
    } else {
        return false_str;
    }
}

type_object bool_type_object = {
    &base_type_object,
    "bool",
    default_hash,
    default_equals,
    NULL,
    NULL,
    NULL,
    NULL,
    bool_str,
};

bool_object bool_true = {
    &bool_type_object,
};

bool_object bool_false = {
    &bool_type_object,
};

bool_object *p_bool_true = &bool_true;
bool_object *p_bool_false = &bool_false;

