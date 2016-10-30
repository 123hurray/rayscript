#include <list_object.h>
#include <allocator.h>


#define LIST_DEFAULT_SIZE 16
#define LIST_LOAD_FACTOR 0.75
#define LIST_SCALE_FACTOR 1.5

#define AS_LIST(o) (((list_object *)(o)))

list_object *new_list_object(long size) {
    list_object *l = R_MALLOC(list_object);
    if(size < LIST_DEFAULT_SIZE) {
        size = LIST_DEFAULT_SIZE;
    }
    l->allocated = size;
    l->size = 0;
    l->table = R_MALLOC_N(ray_object *, size);
    memset(l->table, 0, sizeof(ray_object *) * size);
    return l;
}
ray_object * list_get(ray_object* self, long index) {
    list_object* l = AS_LIST(self);
    if(index >= l->size) {
        return AS_OBJ(nil);
    }
    return l->table[index];
}
long list_append(ray_object *self, ray_object *item) {
    list_object* l = AS_LIST(self);
    if(l->size >= l->allocated * LIST_SCALE_FACTOR) {
       long new_size = l->size * LIST_SCALE_FACTOR; 
       l->allocated = new_size;
       l->table = R_REALLOC(l->table, new_size * sizeof(ray_object *));
    }
    l->table[l->size] = item;
    return (l->size)++;
}
long list_remove(ray_object *self, long index) {
    list_object* l = AS_LIST(self);
    if(index >= l->size) {
        return 0;
    }
    memcpy(&(l->table[index]), &(l->table[index+1]), l->size - index - 1);
    --(l->size);
    return 1;
}
long list_find(ray_object* self, ray_object* item) {
    list_object* l = AS_LIST(self);
    for(long i = 0; i < l->size; ++i) {
        ray_object* v = l->table[i];
        if(OBJ_EQ(v, item)) {
            return i;
        }
    }
    return -1;
}
