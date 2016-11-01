#include <bool_object.h>
string_object* true_str;
string_object* false_str; 

void init_bool_object() {
    true_str = new_string_object("true");
    false_str = new_string_object("false");
}

string_object* bool_str(ray_object* self) {
    if(OBJ_IS_TRUE(self)) {
        INC_REF(true_str);
        return true_str;
    } else {
        INC_REF(false_str);
        return false_str;
    }
}

type_object bool_type_object = {
    INIT_HEADER(&base_type_object),
    "bool",
    default_hash,
    default_equals,
    DEFAULT_BIN_OPS,
    bool_str,
    NULL, 
    default_destruct,
};

bool_object bool_true = {
    INIT_HEADER(&bool_type_object),
};

bool_object bool_false = {
    INIT_HEADER(&bool_type_object),
};

bool_object *p_bool_true = &bool_true;
bool_object *p_bool_false = &bool_false;

