#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>
typedef unsigned char uchar;
typedef struct _type_object type_object;
#define OBJ_HEADER \
    type_object* type;\
    int size;
typedef struct {
    OBJ_HEADER;
} ray_object;

/** nil **/

typedef struct _nil_object nil_object;

extern nil_object* nil;


typedef struct _bool_object bool_object;
extern bool_object bool_true, bool_false;



typedef long (* hash_func)(ray_object *);
typedef bool_object* (*equals_func)(ray_object *, ray_object *);

long default_hash(ray_object *);

bool_object* default_equals(ray_object *, ray_object *);

struct _type_object{
    OBJ_HEADER;
    char* name;
    hash_func __hash;
    equals_func __equals;
};


typedef struct {
    OBJ_HEADER;
    double val;
} number_object;

typedef struct {
    OBJ_HEADER;
    char* val;
    uchar is_hash_cached;
    long hash_cache;
} string_object;

extern type_object base_type_object;
extern type_object number_type_object;
extern type_object string_type_object;






#define NEW_OBJ(type) ((type*)malloc(sizeof(type)))
#define INIT_OBJ_HEADER(obj, type_object, s) do {\
    (obj)->type = &(type_object); \
    (obj)->size = (s); \
} while(0);
        






/** string **/

string_object *new_string_object(char* val);
#define STRING_EXACT(o) ((((ray_object *)o)->type == &string_type_object)?1:0)

#define STRING_OBJ_AS_STRING(o) (((string_object *)o)->val)




/** number **/

number_object *new_number_object(double val);

#define NUMBER_EXACT(o) ((((ray_object *)o)->type == &number_type_object)?1:0)

#define NUMBER_OBJ_AS_NUMBER(o) (((number_object*)o)->val)




#endif
