#include <number_object.h>
#include <stdio.h>

long number_object_hash(ray_object* self) {
    return (long)NUMBER_OBJ_AS_NUMBER(self);
}

#define AS_NUMBER(o) ((number_object*)(o))

string_object* number_str(ray_object* self) {
    char s[256];
    if(OBJ_IS_TRUE(IS_NUMBER_TYPE_LONG(self))) {
        sprintf(s, "%ld", AS_NUMBER(self)->val.l);
    } else {
        sprintf(s, "%lf", AS_NUMBER(self)->val.d);
    }
    return new_string_object(s); 
}

int number_equals(ray_object* self, ray_object* other) {
    if(self == other) {
        return CMP_RELATIONAL_EQ;
    }

    if(OBJ_IS_TRUE(IS_NUMBER_TYPE_LONG(self))
            && OBJ_IS_TRUE(IS_NUMBER_TYPE_LONG(other))) {
        long a = AS_NUMBER(self)->val.l;
        long b = AS_NUMBER(other)->val.l;
        if(a == b) {
            return CMP_RELATIONAL_EQ;
        } else if (a > b) {
            return CMP_GT;
        } else {
            return CMP_LT;
        }
    } else {
        double a = NUMBER_OBJ_AS_NUMBER(self);
        double b = NUMBER_OBJ_AS_NUMBER(other);
        if(a == b) {
            return CMP_RELATIONAL_EQ;
        } else if (a > b) {
            return CMP_GT;
        } else {
            return CMP_LT;
        }
    }
}


// operator functions


ray_object* number_add(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        INC_REF(nil);
        return (ray_object*)nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) + NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) + NUMBER_OBJ_AS_NUMBER(other));
    }
}

ray_object* number_sub(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        INC_REF(nil);
        return (ray_object*)nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) - NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) - NUMBER_OBJ_AS_NUMBER(other));
    }
}


ray_object* number_mul(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        INC_REF(nil);
        return (ray_object*)nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) * NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) * NUMBER_OBJ_AS_NUMBER(other));
    }
}



ray_object* number_div(ray_object* self, ray_object* other) {
    if(OBJ_IS_FALSE(NUMBER_EXACT(self)) || OBJ_IS_FALSE(NUMBER_EXACT(other))) {
        INC_REF(nil);
        return (ray_object*)nil;
    }
    if(IS_NUMBER_TYPE_LONG(self) == p_bool_true && IS_NUMBER_TYPE_LONG(other) == p_bool_true) {
        return (ray_object*)new_number_object_from_long(NUMBER_OBJ_AS_NUMBER(self) / NUMBER_OBJ_AS_NUMBER(other));
    } else {
        return (ray_object*)new_number_object_from_double(NUMBER_OBJ_AS_NUMBER(self) / NUMBER_OBJ_AS_NUMBER(other));
    }
}



number_object *new_number_object_from_double(double val) {
    number_object * n = NEW_OBJ(number_object);
    INIT_OBJ_HEADER(n, number_type_object);
    n->t = T_DOUBLE;
    n->val.d = val;
    return n;
}

number_object *new_number_object_from_long(long val) {
    number_object * n = NEW_OBJ(number_object);
    INIT_OBJ_HEADER(n, number_type_object);
    n->t = T_LONG;
    n->val.l = val;
    return n;
}

void destruct_number_object(ray_object* o) {
    number_object* self = (number_object*)o;
    R_FREE(self);
}

type_object number_type_object = {
    INIT_HEADER(&base_type_object),
    "number",
    number_object_hash,
    number_equals,
    number_add,
    number_sub,
    number_mul,
    number_div,
    number_str,
    NULL,
    destruct_number_object,
};
