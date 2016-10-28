#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>
#include <all_objects.h>
#include <globals.h>

#define OBJ_HEADER \
    type_object* type;\
    int size;

#include <type_object.h>

struct _ray_object {
    OBJ_HEADER;
};


#include <nil_object.h>
#include <bool_object.h>
#include <string_object.h>




long default_hash(ray_object *);

bool_object* default_equals(ray_object *, ray_object *);



struct _number_object {
    OBJ_HEADER;
    double val;
};


extern type_object number_type_object;






#define NEW_OBJ(type) ((type*)malloc(sizeof(type)))
#define INIT_OBJ_HEADER(obj, type_object, s) do {\
    (obj)->type = &(type_object); \
    (obj)->size = (s); \
} while(0);
        








/** number **/

number_object *new_number_object(double val);

#define NUMBER_EXACT(o) ((((ray_object *)o)->type == &number_type_object)?1:0)

#define NUMBER_OBJ_AS_NUMBER(o) (((number_object*)o)->val)




#endif
