#include <list_object.h>
#include <allocator.h>


#define LIST_DEFAULT_SIZE 16
#define LIST_LOAD_FACTOR 0.75
#define LIST_SCALE_FACTOR 1.5

#define AS_LIST(o) (((list_object *)(o)))

void destruct_list_object(ray_object* o) {
    list_object* self = AS_LIST(o);
    int size = self->size;
    for(int i = 0; i < size; ++i) {
        DEC_REF(self->table[i]);
    }
    R_FREE(self->table);
    R_FREE(self);
}

type_object list_type_object = {
    INIT_HEADER(&base_type_object),
    "list",
    default_hash,
    default_equals,
    DEFAULT_BIN_OPS,
    default_str,
    NULL, 
    destruct_list_object,
};

list_object *new_list_object(long size) {
    list_object *l = NEW_OBJ(list_object);
    INIT_OBJ_HEADER(l, list_type_object);
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
        return NULL;
    }
    ray_object* val = l->table[index];
    return val;
}

void list_ensure_size(list_object* l) {
    if(l->size >= l->allocated * LIST_SCALE_FACTOR) {
       long new_size = l->size * LIST_SCALE_FACTOR; 
       l->allocated = new_size;
       l->table = R_REALLOC(l->table, new_size * sizeof(ray_object *));
    }
}

long list_append(ray_object *self, ray_object *item) {
    list_object* l = AS_LIST(self);
    list_ensure_size(l);
    INC_REF(item);
    l->table[l->size] = item;
    return (l->size)++;
}



long list_insert(ray_object *self, long index, ray_object* item) {
    list_object* l = AS_LIST(self);
    if(index > l->size) {
        return 0;
    }
    if(index == l->size) {
        return list_append(self, item);
    }
    list_ensure_size(l);
    memmove(&(l->table[index+1]), &(l->table[index]), sizeof(ray_object*) * (l->size - index));
    INC_REF(item);
    l->table[index] = item;
    ++(l->size);
    return 1;
}
long list_remove(ray_object *self, long index) {
    list_object* l = AS_LIST(self);
    if(index >= l->size) {
        return 0;
    }
    ray_object* item = l->table[index];
    memcpy(&(l->table[index]), &(l->table[index+2]), sizeof(ray_object*) * (l->size - index - 1));
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
