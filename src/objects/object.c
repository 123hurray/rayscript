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
ray_object* default_bin_op(ray_object *self, ray_object* other) {
    R_ERROR("Unsupported.\n");
    INC_REF(nil);
    return AS_OBJ(nil);
}

void default_destruct(ray_object* dummy){}


string_object* default_str(ray_object* self) {
    char s[256];
    sprintf(s, "[%s]", OBJ_GET_TYPE_NAME(self));
    return new_string_object(s);
}

void destruct_ast_node(ray_object* o);
type_object ast_type_object = {
    INIT_HEADER(&base_type_object),
    "ast",
    default_hash,
    default_equals,
    DEFAULT_BIN_OPS,
    default_str,
    NULL,
    destruct_ast_node
};


