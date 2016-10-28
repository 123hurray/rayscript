#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>
#include <all_objects.h>
#include <globals.h>

#define OBJ_HEADER \
    type_object* type;
#define OBJ_HEADER_WITH_SIZE \
    OBJ_HEADER;\
    int size;

#include <type_object.h>

struct _ray_object {
    OBJ_HEADER;
};


#include <nil_object.h>
#include <bool_object.h>
#include <number_object.h>
#include <string_object.h>




long default_hash(ray_object *);

bool_object* default_equals(ray_object *, ray_object *);











#define NEW_OBJ(type) ((type*)malloc(sizeof(type)))
#define INIT_OBJ_HEADER(obj, type_object, s) do {\
    (obj)->type = &(type_object); \
} while(0)
        
#define INIT_OBJ_VAR_HEADER(obj, type_object, s) do {\
    (obj)->type = &(type_object); \
    (obj)->size = (s); \
} while(0)



#define OBJ_GET_TYPE_NAME(o) (((ray_object*)(o))->type->name)

#define OBJ_IS_TRUE(o) ((bool_object*)(o) == p_bool_true)
#define OBJ_IS_FALSE(o) ((bool_object*)(o) == p_bool_false)
#define OBJ_IS_NIL(o) ((o) == (ray_object*)&nil)


#define OBJ_ADD(o1, o2) ((o1)->type->__add((ray_object*)(o1), (ray_object*)(o2))) 
#define OBJ_SUB(o1, o2) ((o1)->type->__sub((ray_object*)(o1), (ray_object*)(o2))) 
#define OBJ_MUL(o1, o2) ((o1)->type->__mul((ray_object*)(o1), (ray_object*)o2)) 
#define OBJ_DIV(o1, o2) ((o1)->type->__div((ray_object*)(o1), (ray_object*)o2)) 
#define OBJ_EQUALS(o1, o2) ((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2))) 







#endif
