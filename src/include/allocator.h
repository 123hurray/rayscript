#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#define _R_MALLOC(s) malloc(s)
#define _R_FREE(p) free(p)
//#define MEM_DEBUG
#ifdef MEM_DEBUG
#define R_MALLOC(type) (printf("Allocate %d %s:%d\n", (int)(sizeof(type)), __FILE__, __LINE__),(type *)_R_MALLOC(sizeof(type)))
#define R_MALLOC_N(type, N) (printf("Allocate %d  %s:%d\n", (int)((N)*sizeof(type)), __FILE__, __LINE__),(type *)_R_MALLOC(sizeof(type) * (N)))
#else
#define R_MALLOC(type) (type *)_R_MALLOC(sizeof(type))
#define R_MALLOC_N(type, N) (type *)_R_MALLOC(sizeof(type) * (N))
#endif

#define R_FREE(p) free(p)
#define R_REALLOC(p, size) realloc((p), (size));

void * r_malloc(size_t size);
void r_free(void * p);
void debug_memory(size_t size);


#endif
