#ifndef LIST_OBJECT_H
#define LIST_OBJECT_H
#include "object.h"
#include <memory.h>

struct _list_object {
    OBJ_HEADER;
    int size;
    ray_object ** table;
    int allocated;
};

#define OBJ_AS_MAP_OBJ(obj) ((map_object*)(obj))









list_object *new_list_object(long size);
ray_object * list_get(ray_object* self, long index);
long list_append(ray_object *self, ray_object *item);
long list_remove(ray_object *self, long index);
long list_find(ray_object *self, ray_object* item);


#endif
