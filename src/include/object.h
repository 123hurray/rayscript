#ifndef OBJECT_H
#define OBJECT_H
#include <stdio.h>
#include <stdlib.h>
#include <all_objects.h>
#include <globals.h>
#include <allocator.h>



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
    type_object* type;\
    int refcnt;
#define OBJ_HEADER_WITH_SIZE \
    OBJ_HEADER;\
    int size;

#define INIT_HEADER(type) type, 1


#include <type_object.h>

struct _ray_object {
    OBJ_HEADER;
};

#define ENABLE_GC
//#define GC_DEBUG
#ifdef GC_DEBUG
#define R_DEBUG_GC(v, ...) printf(v, ##__VA_ARGS__)
#else
#define R_DEBUG_GC(v, ...)
#endif


#ifdef ENABLE_GC

#define INC_REF(o) do {\
    if(o != NULL) {\
        (o)->refcnt++;\
        R_DEBUG_GC("INC_REF:%s %p %d in %s:%d\n", OBJ_GET_TYPE_NAME((o)), (o), (o)->refcnt, __FILE__,__LINE__);\
    }\
}while(0)

#define DEC_REF(o) do {\
    if((o) != NULL) {\
        (o)->refcnt--; \
        R_DEBUG_GC("DEC_REF:%s %p %d in %s:%d\n", OBJ_GET_TYPE_NAME(o), (o), (o)->refcnt, __FILE__,__LINE__);\
        /*assert((o)->refcnt >=0);*/\
        if((o)->refcnt == 0) {\
            (o)->type->__destruct(AS_OBJ(o));\
        }\
    }\
}while(0)
#else
#define INC_REF(o) 
#define DEC_REF(o)
#endif

#include <nil_object.h>
#include <bool_object.h>
#include <number_object.h>
#include <string_object.h>
#include <map_object.h>
#include <list_object.h>
#include <function_object.h>
#include <assert.h>
extern type_object ast_type_object;

void init_objects();


long default_hash(ray_object *);

int default_equals(ray_object *, ray_object *);

string_object* default_str();


ray_object* default_bin_op(ray_object*, ray_object*);
void default_destruct(ray_object*);

#define DEFAULT_BIN_OPS \
    default_bin_op,\
    default_bin_op,\
    default_bin_op,\
    default_bin_op



#define AS_OBJ(o) (((ray_object *)(o)))


#define NEW_OBJ(type) (R_MALLOC(type))
#define INIT_OBJ_HEADER(obj, type_object) do {\
    (obj)->type = &(type_object);\
    (obj)->refcnt = 1;\
} while(0)
        
#define INIT_OBJ_VAR_HEADER(obj, type_object, s) do {\
    (obj)->type = &(type_object); \
    (obj)->size = (s); \
} while(0)



#define OBJ_GET_TYPE_NAME(o) (((ray_object*)(o))->type->name)

#define OBJ_IS_TRUE(o) ((bool_object*)(o) == p_bool_true)
#define OBJ_IS_FALSE(o) ((bool_object*)(o) == p_bool_false)
#define OBJ_IS_NIL(o) ((o) == (ray_object*)nil)

#define OBJ_HASH(o) ((o)->type->__hash(AS_OBJ(o)))

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

#define OBJ_INVOKE(o, l) ((ray_object*)(o))->type->__invoke((o), (l))

#define OBJ_BOOL_FROM_INT(i) (((i)==0)?p_bool_false:p_bool_true)


#define OBJ_STR(o) ((o)->type->__str(o))



#endif
