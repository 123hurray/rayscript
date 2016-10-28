#ifndef NUMBER_OBJECT_H
#define NUMBER_OBJECT_H
#include<object.h>

extern type_object number_type_object;

typedef enum {
    T_LONG,
    T_DOUBLE
} number_type;
struct _number_object {
    OBJ_HEADER;
    number_type t;
    union {
        long l;
        double d;
    } val;
};

number_object *new_number_object_from_double(double val);
number_object *new_number_object_from_long(long val);

#define IS_NUMBER_TYPE_LONG(o) ((((number_object*)(o))->t == T_LONG)?p_bool_true:p_bool_false)

#define NUMBER_EXACT(o) ((((ray_object *)(o))->type == &number_type_object)?p_bool_true:p_bool_false)

#define NUMBER_OBJ_AS_NUMBER(o) (\
        (IS_NUMBER_TYPE_LONG(o) == p_bool_false)?\
         (((number_object*)(o))->val.d)\
        : (((double)((number_object*)(o))->val.l))\
        )


#endif // NUMBER_OBJECT_H
