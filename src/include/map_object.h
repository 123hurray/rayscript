#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H
#include "object.h"
#include <memory.h>
typedef struct _entry_object {
    OBJ_HEADER;
    long hash;
    ray_object *key;
    ray_object *val;
    struct _entry_object *next;
} entry_object;


typedef struct {
    OBJ_HEADER;
    entry_object ** table;
    int allocated;
} map_object;

#define OBJ_AS_MAP_OBJ(obj) ((map_object*)(obj))









map_object *new_map_object(int size);
ray_object * map_get(ray_object* self, ray_object *key);
void map_put(ray_object *self, ray_object *key, ray_object *val);



#endif