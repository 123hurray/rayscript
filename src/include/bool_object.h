#ifndef BOOL_OBJECT_H
#define BOOL_OBJECT_H
#include <object.h>

struct _bool_object {
    OBJ_HEADER;
};

type_object bool_type_object;

extern bool_object *p_bool_true;
extern bool_object *p_bool_false;

extern bool_object bool_true;
extern bool_object bool_false;

#endif // BOOL_OBJECT_H
