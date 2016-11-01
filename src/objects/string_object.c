#include <string_object.h>
#include <allocator.h>
#include <string.h>

long string_object_hash(ray_object *self) {
    string_object *str_obj = (string_object *)self;
    if(str_obj->is_hash_cached) {
        return str_obj->hash_cache;
    }
    long hash = 0;
    int len = str_obj->size;
    char* str = STRING_OBJ_AS_STRING(str_obj);
    for(int i = 0; i < len; ++i) {
        hash = hash * 31 + str[i++];
    }
    str_obj->hash_cache = hash;
    str_obj->is_hash_cached = 1;
    return hash;
}

int string_object_equals(ray_object * self, ray_object * other) {
    if(self == NULL) {
        return (other == NULL)?CMP_RELATIONAL_EQ:CMP_LT;
    }
    if(other == NULL) {
        return CMP_GT;
    }
    if(OBJ_IS_FALSE(STRING_EXACT(other))) {
        return CMP_NOT_RELATIONAL_NE;
    }
    int val = strcmp(STRING_OBJ_AS_STRING(self), STRING_OBJ_AS_STRING(other));
    if(val == 0) {
        return CMP_RELATIONAL_EQ;
    } else if(val < 0) {
        return CMP_LT;
    } else {
        return CMP_GT;
    }
    
}

string_object* string_str(ray_object* self) {
    return new_string_object(STRING_OBJ_AS_STRING(self));
}

void destruct_string_object(ray_object* o) {
    string_object* self = (string_object*)o;
    R_FREE(self->val);
    R_FREE(self);
}
type_object string_type_object = {
    INIT_HEADER(&base_type_object),
    "string",
    string_object_hash,
    string_object_equals,
    DEFAULT_BIN_OPS,
    string_str,
    NULL,
    destruct_string_object
};
string_object *new_string_object(char * str) {
    string_object *str_obj = NEW_OBJ(string_object);
    INIT_OBJ_HEADER(str_obj, string_type_object);
    str_obj->size = strlen(str) + 1;
    str_obj->val = R_MALLOC_N(char, str_obj->size);
    strcpy(str_obj->val, str);
    str_obj->is_hash_cached = 0;
    return str_obj;
}

