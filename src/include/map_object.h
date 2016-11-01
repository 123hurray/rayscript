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


struct _map_object {
    OBJ_HEADER;
    int size;
    entry_object ** table;
    int allocated;
};

#define OBJ_AS_MAP_OBJ(obj) ((map_object*)(obj))









void destruct_map_object(ray_object*);
map_object *new_map_object(int size);
ray_object * map_get(ray_object* self, ray_object *key);
void map_put(ray_object *self, ray_object *key, ray_object *val);



#endif
