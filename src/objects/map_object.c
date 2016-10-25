#include "map_object.h"

type_object entry_object_type = {
    &base_type_object,
    1,
    "entry",
    type_object_hash
};

type_object map_object_type = {
    &base_type_object,
    1,
    "map",
    type_object_hash
};

#define MAP_DEFAULT_SIZE 16
#define MAP_LOAD_FACTOR 0.75
#define MAP_SCALE_FACTOR 1.5

map_object *new_map_object(int size) {
    map_object * map = NEW_OBJ(map_object);
    INIT_OBJ_HEADER(map, map_object_type, 0);
    if(size < 1) {
        size = 1;
    }
    map->allocated = size;
    map->table = (entry_object **)malloc(sizeof(entry_object*)*size);
    return map;
}


entry_object *new_entry_object(ray_object *key, ray_object *val) {
    entry_object * entry = NEW_OBJ(entry_object);
    INIT_OBJ_HEADER(entry, entry_object_type, 1);
    entry->key = key;
    entry->val = val;
    entry->hash = key->type->__hash(key);
    entry->next = NULL;
    return entry;
}


void map_put(ray_object *self, ray_object *key, ray_object *val) {
    map_object *map = OBJ_AS_MAP_OBJ(self);
    entry_object *entry = new_entry_object(key, val);
    long hash = entry->hash;
    if(map->size >= map->allocated * MAP_LOAD_FACTOR) {
        int old_size = map->allocated;
        map->allocated *= MAP_SCALE_FACTOR;
        int new_size = map->allocated;
        entry_object **new_table = (entry_object **)malloc(new_size * sizeof(entry_object *));
        memset(new_table, 0, new_size);
        for(int i = 0; i < old_size; ++i) {
            entry_object* e = (entry_object *)map->table[i];
            while(e) {
                int index = e->hash % new_size;
                entry_object * save = e->next;
                if(new_table[index] == NULL) {
                    new_table[index] = save;
                    save->next = NULL;
                } else {
                    save->next = new_table[index];
                    new_table[index] = save;
                }
                e = e->next;
            }
        }
        free(map->table);
        map->table = new_table;
    }
    int index = entry->hash % map->allocated;
    entry->next = map->table[index];
    map->table[index] = entry;
}


ray_object * map_get(ray_object* self, ray_object *key) {
    map_object *map = OBJ_AS_MAP_OBJ(self);
    long hash = key->type->__hash(key);
    int index = hash % map->allocated;
    entry_object* e = map->table[index];
    while(e) {
        if(e->hash == hash && e->key->type->__equals(e->key, key)) {
            return e->val;
        }
        e = e->next;
    }
    return NULL;

}
