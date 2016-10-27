#include "object.h"
#include <stdlib.h>
#include <string.h>
#include "map_object.h"

long default_hash(ray_object *self) {
    return (long)((void *)self);
}

bool_object* default_equals(ray_object *self, ray_object* other) {
    return (self == other)?&bool_true:&bool_false;
}

type_object base_type_object = {
    &base_type_object,
    5,
    "type",
    default_hash  
};

/** nil **/

type_object nil_type_object = {
    &base_type_object,
    0,
    "nil",
    default_hash
};

typedef struct _nil_object {
    OBJ_HEADER;
} nil_object;

nil_object _nil = {
    &nil_type_object,
    1
};

nil_object * nil = &_nil;



/** bool **/


type_object bool_type_object = {
    &base_type_object,
    0,
    "bool",
    default_hash,
    default_equals
};

struct _bool_object {
    OBJ_HEADER;
};

bool_object bool_true = {
    &bool_type_object,
    0
};


bool_object bool_false = {
    &bool_type_object,
    0
};

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





















/** string **/

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

bool_object *string_object_equals(ray_object * self, ray_object * other) {
    if(self == NULL) {
        return (other == NULL)?&bool_true:&bool_false;
    }
    if(other == NULL) {
        return &bool_false;
    }
    return (strcmp(STRING_OBJ_AS_STRING(self), STRING_OBJ_AS_STRING(other)) == 0)?&bool_true:&bool_false;
}

type_object string_type_object = {
    &base_type_object,
    7,
    "string",
    string_object_hash,
    string_object_equals
};
string_object *new_string_object(char * str) {
    string_object *str_obj = (string_object *)malloc(sizeof(string_object));
    str_obj->type = &string_type_object;
    str_obj->size = strlen(str);
    str_obj->val = (char *)malloc(str_obj->size);
    strcpy(str_obj->val, str);
    str_obj->is_hash_cached = 0;
    return str_obj;
}
