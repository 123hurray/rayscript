#include <number_object.h>

long number_object_hash(ray_object* self) {
    return (long)NUMBER_OBJ_AS_NUMBER(self);
}

#define AS_NUMBER(o) ((number_object*)(o))

bool_object* number_equals(ray_object* self, ray_object* other) {
    if(self == other) {
        return p_bool_true;
    }

    if(OBJ_IS_TRUE(IS_NUMBER_TYPE_LONG(self))
            && OBJ_IS_TRUE(IS_NUMBER_TYPE_LONG(other))) {
        return (AS_NUMBER(self)->val.l == AS_NUMBER(other)->val.l)?p_bool_true:p_bool_false;
    } else {
        return (NUMBER_OBJ_AS_NUMBER(self) == NUMBER_OBJ_AS_NUMBER(other))?p_bool_true:p_bool_false;
    }
}


// operator functions


ray_object* number_add(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        return (ray_object*)&nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) + NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) + NUMBER_OBJ_AS_NUMBER(other));
    }
}

ray_object* number_sub(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        return (ray_object*)&nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) - NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) - NUMBER_OBJ_AS_NUMBER(other));
    }
}


ray_object* number_mul(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        return (ray_object*)&nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) * NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) * NUMBER_OBJ_AS_NUMBER(other));
    }
}



ray_object* number_div(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        return (ray_object*)&nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) / NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) / NUMBER_OBJ_AS_NUMBER(other));
    }
}



number_object *new_number_object_from_double(double val) {
    number_object * n = (number_object *)malloc(sizeof(number_object));
    n->type = &number_type_object;
    n->t = T_DOUBLE;
    n->val.d = val;
    return n;
}

number_object *new_number_object_from_long(long val) {
    number_object * n = (number_object *)malloc(sizeof(number_object));
    n->type = &number_type_object;
    n->t = T_LONG;
    n->val.l = val;
    return n;
}

type_object number_type_object = {
    &base_type_object,
    "number",
    number_object_hash,
    number_equals,
    number_add,
    number_sub,
    number_mul,
    number_div,
};
