#ifndef NIL_OBJECT_H
#define NIL_OBJECT_H
#include <object.h>
#include <type_object.h>

typedef struct {
    OBJ_HEADER;  
} nil_object;

void init_nil_object();
extern type_object nil_type_object;
extern nil_object* nil;

#endif // NIL_OBJECT_H
