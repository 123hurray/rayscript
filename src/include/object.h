#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>
#include <all_objects.h>
#include <globals.h>

/*
 *
 * 8 7 6 5 4 3 2 1
 * | | | | | | | |
 * | | | | | | | |__equality enable
 * | | | | | | |____relational enable
 * | | | | | |______equals
 * | | | | |________less than
 * | | | |__________great than
 * | | |____________less equal than
 * | |______________great equal than
 * |________________unequals
 */

#define FEATURE_EQUALITY    0x1
#define FEATURE_RELATIONAL  0x2
#define BIT_EQ 0x04
#define BIT_NE 0x80
#define BIT_LT 0x08
#define BIT_GT 0x10
#define BIT_LE 0x20
#define BIT_GE 0x40



#define CMP_LT (BIT_NE | BIT_LE | BIT_LT | FEATURE_RELATIONAL | FEATURE_EQUALITY)
#define CMP_GT (BIT_NE | BIT_GE | BIT_GT | FEATURE_RELATIONAL | FEATURE_EQUALITY)

#define CMP_RELATIONAL_EQ (CMP_NOT_RELATIONAL_EQ | BIT_LE | BIT_GE | FEATURE_RELATIONAL)


#define CMP_LE (BIT_LE | FEATURE_RELATIONAL | FEATURE_EQUALITY)
#define CMP_GE (BIT_GE | FEATURE_RELATIONAL | FEATURE_EQUALITY) 
#define CMP_NOT_RELATIONAL_EQ (BIT_EQ | FEATURE_EQUALITY)
#define CMP_NOT_RELATIONAL_NE (BIT_NE | FEATURE_EQUALITY)
#define CMP_RELATIONAL_NE (BIT_NE | CMP_NOT_RELATIONAL_NE | FEATURE_RELATIONAL)
#define CMP_EQ (BIT_EQ | FEATURE_EQUALITY)
#define CMP_NE (BIT_NE | FEATURE_EQUALITY)
#define CMP_UNSUPPORTED 0

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
#include <map_object.h>
#include <list_object.h>

void init_objects();


long default_hash(ray_object *);

int default_equals(ray_object *, ray_object *);

string_object* default_str();






#define AS_OBJ(o) (((ray_object *)(o)))


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
#define OBJ_IS_NIL(o) ((o) == (ray_object*)nil)


#define OBJ_ADD(o1, o2) ((o1)->type->__add((ray_object*)(o1), (ray_object*)(o2))) 
#define OBJ_SUB(o1, o2) ((o1)->type->__sub((ray_object*)(o1), (ray_object*)(o2))) 
#define OBJ_MUL(o1, o2) ((o1)->type->__mul((ray_object*)(o1), (ray_object*)o2)) 
#define OBJ_DIV(o1, o2) ((o1)->type->__div((ray_object*)(o1), (ray_object*)o2)) 
#define OBJ_EQ(o1, o2) (((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2)) & CMP_EQ) == CMP_EQ) 
#define OBJ_NE(o1, o2) (((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2)) & CMP_NE) == CMP_NE) 
#define OBJ_GT(o1, o2) (((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2)) & CMP_GT) == CMP_GT) 
#define OBJ_LT(o1, o2) (((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2)) & CMP_LT) == CMP_LT) 
#define OBJ_LE(o1, o2) (((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2)) & CMP_LE) == CMP_LE) 
#define OBJ_GE(o1, o2) (((o1)->type->__equals((ray_object*)(o1), (ray_object*)(o2)) & CMP_GE) == CMP_GE) 


#define OBJ_BOOL_FROM_INT(i) (((i)==0)?p_bool_false:p_bool_true)


#define OBJ_STR(o) ((o)->type->__str(o))



#endif
