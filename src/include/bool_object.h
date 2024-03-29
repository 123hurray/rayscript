#ifndef BOOL_OBJECT_H
#define BOOL_OBJECT_H
#include <object.h>

struct _bool_object {
    OBJ_HEADER;
};

void init_bool_object();

type_object bool_type_object;

extern bool_object *p_bool_true;
extern bool_object *p_bool_false;

extern bool_object bool_true;
extern bool_object bool_false;


#define BOOL_EXACT(o) ((((ray_object*)o)->type == &bool_type_object)?p_bool_true:p_bool_false)

#endif // BOOL_OBJECT_H
