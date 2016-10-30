#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#define R_MALLOC(type) ((type *)malloc(sizeof(type)))
#define R_MALLOC_N(type, N) ((type *)malloc(sizeof(type) * (N)))

#define R_FREE(p) free(p)
#define R_REALLOC(p, size) realloc((p), (size));

void * r_malloc(size_t size);
void r_free(void * p);
void debug_memory(size_t size);


#endif
